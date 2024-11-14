#pragma once
#include "Lexer.h"
#include "Token.h"
#include <string>
//#include <set>
#include "Situation.h"
#include <fstream>
//#include <functional>
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
	//Èìÿ ôàéëà, èìÿ ôàéëà äëÿ îøèáîê, ãðàììàòèêà, ðåæèì çàïèñè (äîçàïèñü â ôàéë îøèáîê èëè íîâàÿ çàïèñü)
	parser(const std::string name_file, const std::string out_file, const std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>>&grammar, const bool add_or_not = 0)
		:name_file{ name_file }, out_file{ out_file }, grammar{ grammar },
		out{ out_file, add_or_not ? std::ios::app : std::ios::out }, lex{ name_file, out_file, 1 } {}

private:

	bool find(const std::vector<situation>& vec, const situation& a) const {
		for (int i = 0; i < vec.size(); ++i) {
			if (vec[i] == a) {
				return true;
			}
		}
		return false;
	}

	void Scan(std::vector<std::vector<situation>>& D, const token& lexem) {
		unsigned D_size = D.size();
		unsigned S_size = D[D_size - 1].size();
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() &&
				grammar[i.rule][i.sub_rule][i.dot].first != "N") // Åñëè òî÷êà íå ñòîèò â êîíöå ñèòóàöèè è òî÷êà ñòîèò ïåðåä òåðìèíàëîì
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONST" && lexem.getType() == "CONST"))) { // Åñëè òåðìèíàë ðàâåí ëåêñåìå òî âñòàâëÿåì åãî â ìíîæåñòâî ñèòóàöèé D[D_size-1]
					situation a(i.rule, i.sub_rule, i.dot + 1, i.sym_index);
					if (!find(D[D_size - 1], a)) {
						D[D_size - 1].push_back(a);
					}
				}
			}
		}
		// Åñëè íå îäíà èç ñèòóàöèé èç ïðåä. ìíîæåñòâà íå ïîäîøëà, òî çàïèñûâàþ âñå ñèòóàöèè â òåê. ìíîæåñòâî, 
		// êîòîðûå áû â òåîðèè ìîãëè áû ïîäîéòè åñëè áû âñå áûëî êîððåêòíî
		// È âûâîæó îøèáêó â ôàéë
		if (S_size == D[D_size - 1].size()) {
			for (situation i : D[D_size - 2]) {
				if (i.dot != grammar[i.rule][i.sub_rule].size() &&
					grammar[i.rule][i.sub_rule][i.dot].first != "N")
				{
					situation a(i.rule, i.sub_rule, i.dot + 1, i.sym_index);
					if (!find(D[D_size - 1], a)) {
						D[D_size - 1].push_back(a);
					}
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

	void Predict(std::vector<std::vector<situation>>& D) {
		unsigned D_size = D.size();
		for (int i = 0; i < D[D_size - 1].size(); ++i) {
			situation i_sit(D[D_size - 1][i]);
			if (i_sit.dot != grammar[i_sit.rule][i_sit.sub_rule].size() && grammar[i_sit.rule][i_sit.sub_rule][i_sit.dot].first == "N") {
				unsigned R_index = std::stoi(grammar[i_sit.rule][i_sit.sub_rule][i_sit.dot].second); // íîìåð ïðîäóêöèè
				unsigned R_size = grammar[R_index].size(); // êîëè÷åñòâî ïðàâèë â ïðîäóêöèè
				for (unsigned j = 0; j < R_size; ++j) {
					situation a(R_index, j, 0, D_size - 1);
					if (!find(D[D_size - 1], a)) {
						D[D_size - 1].push_back(a);
					}
					//show(D, D_size - 1);
					//std::cout << j << '\n';
				}
			}
		}
	}

	void Complete(std::vector<std::vector<situation>>& D) {
		unsigned D_size = D.size();
		for (unsigned i = 0; i < D[D_size - 1].size(); ++i) {
			situation i_sit(D[D_size - 1][i]);
			if (i_sit.dot == grammar[i_sit.rule][i_sit.sub_rule].size()) {

				for (unsigned j = 0; j < D[i_sit.sym_index].size(); ++j) {
					//Åñëè òî÷êà íå â êîíöå åñëè îíà ñòîèò ïåðåä òåðìèíàëîì è åñëè ñòîèò ïåðåä íóæíûì ïðàâèëîì
					situation j_sit(D[i_sit.sym_index][j]);
					if (j_sit.dot != grammar[j_sit.rule][j_sit.sub_rule].size()
						&& (grammar[j_sit.rule][j_sit.sub_rule][j_sit.dot].first == "N"
							&& std::stoi(grammar[j_sit.rule][j_sit.sub_rule][j_sit.dot].second) == i_sit.rule)) {
						situation a(j_sit.rule, j_sit.sub_rule, j_sit.dot + 1, j_sit.sym_index);
						if (!find(D[D_size - 1], a)) {
							D[D_size - 1].push_back(a);
						}
					}
				}
			}
		}
	}

	/*void show(std::vector<std::vector<situation>>& D, const unsigned& i) {
		std::cout << "---------------\n";
		for (situation j : D[i]) {
			std::cout << j.rule << " " << j.sub_rule << " " << j.dot << " " << j.sym_index << '\n';
		}
	}*/

public:
	std::vector<std::vector<situation>> algo_Erli() {
		std::vector<std::vector<situation>> parse_list;
		parse_list.push_back(std::vector<situation>());
		parse_list[0].push_back(situation(0, 0, 0, 0));
		unsigned Pars_set_size = 0;
		while (Pars_set_size != parse_list[0].size()) {
			Pars_set_size = parse_list[0].size();
			Predict(parse_list);
			Complete(parse_list);
		}
		//show(parse_list, 0);

		for (unsigned i = 1; !lex.end(); ++i) {
			token next_lexm(lex.next_lexem());
			parse_list.push_back(std::vector<situation>());
			Scan(parse_list, next_lexm);
			Pars_set_size = 0;
			while (Pars_set_size != parse_list[i].size()) {
				Pars_set_size = parse_list[i].size();
				Predict(parse_list);
				Complete(parse_list);
			}
			/*show(parse_list, i);
			std::cout << next_lexm.getLexem() << " " << i << '\n';*/
		}
		/*show(parse_list, parse_list.size());
		std::cout << next_lexm.getLexem() << " " << parse_list.size() << '\n';*/

		return parse_list;
	}
};