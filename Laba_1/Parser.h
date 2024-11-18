#pragma once
#include "Lexer.h"
#include "Token.h"
#include <string>
#include "Situation.h"
#include <fstream>
#include "Tree.h"


class parser {

	std::string name_file;
	std::string out_file;
	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	lexer lex;
	std::ofstream out;
	bool correct = true;

public:

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

	void R(Node* root, const std::vector<std::vector<situation>>& parse_list, unsigned& count_lexms, const situation& a) {
		for (int i = grammar[a.rule][a.sub_rule].size()-1; i >= 0; --i) {
			if (grammar[a.rule][a.sub_rule][i].first == "N") {
				int rule = stoi(grammar[a.rule][a.sub_rule][i].second);
				for (situation j : parse_list[count_lexms]) {
					if (j.rule == rule && j.dot == grammar[j.rule][j.sub_rule].size()) {
						Node* son = new Node(grammar[a.rule][a.sub_rule][i].second);
						root->add(son);
						std::vector<Node*> children = root->getChildren();
						R(children[children.size() - 1], parse_list, count_lexms, j);
					}
				}
			}
			else
			{
				Node* son = new Node(grammar[a.rule][a.sub_rule][i].second);
				root->add(son);
				count_lexms--;
			}
		}
	}

	void Out_errors(const std::vector<situation>& possible_sits, const token& lexem, const bool mode) {
		if (mode) {
			for (situation i : possible_sits) {
				if (grammar[i.rule][i.sub_rule][i.dot].first != "VAR" && grammar[i.rule][i.sub_rule][i.dot].first != "CONST") {
					out << "Missing lexem '" << grammar[i.rule][i.sub_rule][i.dot].second << "' before '" << lexem.getLexem() << "'\n";
				}
				else if (grammar[i.rule][i.sub_rule][i.dot].first == "VAR") {
					out << "Missing variable before '" << lexem.getLexem() << "'\n";
				}
				else {
					out << "Missing constant before '" << lexem.getLexem() << "'\n";
				}
			}
		}
		else {
			for (situation i : possible_sits) {
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

	void Handle_errors(std::vector<std::vector<situation>>& D, const token& lexem) {
		unsigned D_size = D.size();
		std::vector<situation> possible_situations;
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() &&
				grammar[i.rule][i.sub_rule][i.dot].first != "N")
			{

				situation a(i.rule, i.sub_rule, i.dot + 1, i.sym_index);
				if (!find(D[D_size - 1], a)) {
					possible_situations.push_back(i);
					D[D_size - 1].push_back(a);
				}
			}
		}
		unsigned Pars_set_size = 0;
		while (Pars_set_size != D[D_size-1].size()) {
			Pars_set_size = D[D_size-1].size();
			Predict(D);
			Complete(D);
		}
		D.push_back(std::vector<situation>());
		Scan(D, lexem, 0);
		if (D[D.size() - 1].size() != 0) {
			Out_errors(possible_situations, lexem, 1);
		}
		else {
			D.pop_back();
			for (situation i : D[D_size - 2]) {
				if (i.dot != grammar[i.rule][i.sub_rule].size() &&
					grammar[i.rule][i.sub_rule][i.dot].first != "N")
				{
					if (!find(D[D_size - 1], i)) {
						D[D_size - 1].push_back(i);
					}
				}
			}
			Out_errors(possible_situations, lexem, 0);
		}

	}

	void Scan(std::vector<std::vector<situation>>& D, const token& lexem, bool not_error = 1) {
		unsigned D_size = D.size();
		unsigned S_size = D[D_size - 1].size();
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() &&
				grammar[i.rule][i.sub_rule][i.dot].first != "N")
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONST" && lexem.getType() == "CONST"))) { 
					situation a(i.rule, i.sub_rule, i.dot + 1, i.sym_index);
					if (!find(D[D_size - 1], a)) {
						D[D_size - 1].push_back(a);
					}
				}
			}
		}

		if (not_error && S_size == D[D_size - 1].size()) {
			correct = false;
			Handle_errors(D, lexem);
		}
	}

	void Predict(std::vector<std::vector<situation>>& D) {
		unsigned D_size = D.size();
		for (int i = 0; i < D[D_size - 1].size(); ++i) {
			situation i_sit(D[D_size - 1][i]);
			if (i_sit.dot != grammar[i_sit.rule][i_sit.sub_rule].size() && grammar[i_sit.rule][i_sit.sub_rule][i_sit.dot].first == "N") {
				unsigned R_index = std::stoi(grammar[i_sit.rule][i_sit.sub_rule][i_sit.dot].second);
				unsigned R_size = grammar[R_index].size();
				for (unsigned j = 0; j < R_size; ++j) {
					situation a(R_index, j, 0, D_size - 1);
					if (!find(D[D_size - 1], a)) {
						D[D_size - 1].push_back(a);
					}
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

	///*void show(std::vector<std::vector<situation>>& D, const unsigned& i) {
	//	std::cout << "---------------\n";
	//	for (situation j : D[i]) {
	//		std::cout << j.rule << " " << j.sub_rule << " " << j.dot << " " << j.sym_index << '\n';
	//	}
	//}*/

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

		token last_lexm;
		while(!lex.end()) {
			token next_lexm(lex.next_lexem());
			if (next_lexm.getLexem() == "") { break; }
			last_lexm = next_lexm;
			parse_list.push_back(std::vector<situation>());
			Scan(parse_list, next_lexm);
			Pars_set_size = 0;
			while (Pars_set_size != parse_list[parse_list.size()-1].size()) {
				Pars_set_size = parse_list[parse_list.size()-1].size();
				Predict(parse_list);
				Complete(parse_list);
			}
		}

		for (situation i : parse_list[parse_list.size() - 1]) {
			situation finish(0, 0, 1, 0);
			if (i == finish) {
				return parse_list;
			}
		}

		correct = false;
		for (situation i : parse_list[parse_list.size() - 1]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() && grammar[i.rule][i.sub_rule][i.dot].first != "N") {
				out << "Missing lexem: '" << grammar[i.rule][i.sub_rule][i.dot].second << "' after '"
					<< last_lexm.getLexem() << "'\n";
			}
		}

		return parse_list;
	}

	Node* buildTree(const std::vector<std::vector<situation>>& parse_list) {
		if (correct) {
			unsigned count_lexms = parse_list.size()-1;
			Node* root = new Node("0");
			situation start(0, 0, 1, 0);
			R(root, parse_list, count_lexms, start);
			return root;
		}
		else {
			return nullptr;
		}
	}
};