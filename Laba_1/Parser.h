#pragma once
#include "Lexer.h"
#include "Token.h"
#include "string"
#include <set>
#include "Situation.h"
#include <fstream>


class parser {

	std::string name_file;
	std::string out_file;
	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	lexer lex;
	std::ofstream out;

public:
	//��� �����, ��� ����� ��� ������, ����������, ����� ������ (�������� � ���� ������ ��� ����� ������)
	parser(const std::string& name_file, const std::string& out_file, const std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>>&grammar, const bool add_or_not = 0)
		:name_file{ name_file }, out_file{ out_file }, grammar{ grammar },
		out{ out_file, add_or_not ? std::ios::app : std::ios::out }, lex{ name_file, out_file, 1 } {}

private:
	void Scan(std::vector<std::set<situation>>& D, const token& lexem) {
		unsigned D_size = D.size();
		unsigned S_size = D[D_size - 1].size();
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() && 
				grammar[i.rule][i.sub_rule][i.dot].first != "N") // ���� ����� �� ����� � ����� �������� � ����� ����� ����� ����������
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONTS" && lexem.getType() == "CONTS"))) { // ���� �������� ����� ������� �� ��������� ��� � ��������� �������� D[D_size-1]
					D[D_size - 1].insert(situation(i.rule, i.sub_rule, i.dot + 1, i.sym_index));
				}
			}
		}
		// ���� �� ���� �� �������� �� ����. ��������� �� �������, �� ��������� ��� �������� � ���. ���������, 
		// ������� �� � ������ ����� �� ������� ���� �� ��� ���� ���������
		// � ������ ������ � ����
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

	void Predict(std::vector<std::set<situation>>& D, const token& lexem) {

	}
};