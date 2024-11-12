#pragma once
#include "Lexer.h"
#include "Token.h"
#include "string"
#include <set>
#include "Situation.h"
#include <fstream>
//#include <fstream>



class parser {

	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	lexer lex;
	std::ofstream out;
	

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
};