#pragma once
#include "Lexer.h"
#include "Token.h"
#include <string>
#include <set>
#include "Situation.h"
#include <fstream>
#include <functional>
#include <iostream>


class parser {

	std::string name_file;
	std::string out_file;
	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	lexer lex;
	std::ofstream out;

public:
	//Имя файла, имя файла для ошибок, грамматика, режим записи (дозапись в файл ошибок или новая запись)
	parser(const std::string name_file, const std::string out_file, const std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>>&grammar, const bool add_or_not = 0)
		:name_file{ name_file }, out_file{ out_file }, grammar{ grammar },
		out{ out_file, add_or_not ? std::ios::app : std::ios::out }, lex{ name_file, out_file, 1 } {}

private:
	void Scan(std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>>& D, const token& lexem) {
		unsigned D_size = D.size();
		unsigned S_size = D[D_size - 1].size();
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() && 
				grammar[i.rule][i.sub_rule][i.dot].first != "N") // Если точка не стоит в конце ситуации и точка стоит перед терминалом
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONTS" && lexem.getType() == "CONTS"))) { // Если терминал равен лексеме то вставляем его в множество ситуаций D[D_size-1]
					D[D_size - 1].insert(situation(i.rule, i.sub_rule, i.dot + 1, i.sym_index));
				}
			}
		}
		// Если не одна из ситуаций из пред. множества не подошла, то записываю все ситуации в тек. множество, 
		// которые бы в теории могли бы подойти если бы все было корректно
		// И вывожу ошибку в файл
		if (S_size == D[D_size - 1].size()) { 
			for (situation i : D[D_size - 2]) {
				if (i.dot != grammar[i.rule][i.sub_rule].size() &&
					grammar[i.rule][i.sub_rule][i.dot].first != "N") 
				{
					D[D_size - 1].insert(situation(i.rule, i.sub_rule, i.dot + 1, i.sym_index));
					if (grammar[i.rule][i.sub_rule][i.dot].first != "VAR" && grammar[i.rule][i.sub_rule][i.dot].first != "CONST") {
						out << "Assuming lexem '" << grammar[i.rule][i.sub_rule][i.dot].second << "' instead of '" << lexem.getLexem() << "'\n";
					}
					else if (grammar[i.rule][i.sub_rule][i.dot].first == "VAR") {
						out << "Assuming variable instead of '" << lexem.getLexem() << "'\n";
					}
					else {
						out << "Assuming constant instead of '" << lexem.getLexem() << "'\n";
					}
				}
			}
		}
	}

	void Predict(std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>>& D) {
		unsigned D_size = D.size();
		for (situation i : D[D_size - 1]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() && grammar[i.rule][i.sub_rule][i.dot].first == "N") {
				unsigned R_index = std::stoi(grammar[i.rule][i.sub_rule][i.dot].second); // номер продукции
				unsigned R_size = grammar[R_index].size(); // количество правил в продукции
				for (unsigned j = 0; j < R_size; ++j) {
					D[D_size - 1].insert(situation(R_index, j, 0, D_size - 1));
				}
			}
		}
	}

	void Complete(std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>>& D) {
		unsigned D_size = D.size();
		for (situation i : D[D_size - 1]) {
			if (i.dot == grammar[i.rule][i.sub_rule].size()) {
				
				for (situation j : D[i.sym_index]) {
					//Если точка не в конце если она стоит перед терминалом и если стоит перед нужным правилом
					if (j.dot != grammar[j.rule][j.sub_rule].size()
						&& (grammar[j.rule][j.sub_rule][j.dot].first == "N"
							&& std::stoi(grammar[j.rule][j.sub_rule][j.dot].second) == i.rule)) {
						D[D_size - 1].insert(situation(j.rule, j.sub_rule, j.dot + 1, j.sym_index));
					}
				}
			}
		}
	}

	void show(std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>>& D, const unsigned& i) {
		std::cout << "---------------\n";
		for (situation j : D[i]) {
			std::cout << j.rule << " " << j.sub_rule << " " << j.dot << " " << j.sym_index << '\n';
		}
	}

public:
	std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>> algo_Erli() {
		std::vector<std::set<situation, std::function<bool(const situation&, const situation&)>>> parse_list;
		parse_list.push_back(std::set<situation, std::function<bool(const situation&, const situation&)>>(comp_sit));
		parse_list[0].insert(situation(0, 0, 0, 0));
		unsigned Pars_set_size = 0;
		while (Pars_set_size != parse_list[0].size()) {
			Pars_set_size = parse_list[0].size();
			Predict(parse_list);
			Complete(parse_list);
		}

		token next_lexm(lex.next_lexem());
		for (unsigned i = 1; !lex.end(); ++i) {
			parse_list.push_back(std::set<situation, std::function<bool(const situation&, const situation&)>>(comp_sit));
			Scan(parse_list, next_lexm);
			Pars_set_size = 0;
			while (Pars_set_size != parse_list[i].size()) {
				Pars_set_size = parse_list[i].size();
				Predict(parse_list);
				Complete(parse_list);
			}
			next_lexm = lex.next_lexem();
			show(parse_list, i);
		}

		return parse_list;
	}
};