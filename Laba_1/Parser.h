#pragma once
#include "Grammar.h"
#include "Lexer.h"
#include "Token.h"
#include "string"
#include <set>
#include "Situation.h"
//#include <fstream>



class parser {

	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	lexer lex;
	

	void Scan(std::vector<std::set<situation>>& D, const token& lexem) {
		unsigned D_size = D.size();
		unsigned S_size = D[D_size - 1].size();
		for (situation i : D[D_size - 2]) {
			if (i.dot != grammar[i.rule][i.sub_rule].size() && 
				grammar[i.rule][i.sub_rule][i.dot].first != "TERM") // ���� ����� �� ����� � ����� �������� � ����� ����� ����� ����������
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONTS" && lexem.getType() == "CONTS"))) { // ���� �������� ����� ������� �� ��������� ��� � ��������� �������� D[D_size-1]
					D[D_size - 1].insert(situation(i.rule, i.sub_rule, i.dot + 1, i.sym_index));
				}
			}
		}
		if (S_size == D[D_size - 1].size()) { // ���� ������ ��������� �������� �� ��������� ������ �������� ������� �� ������� �� ����� �������� �� ����������� ���������
			// ����� ����� �������� �� ������ � ����������� ��� �������� � D[D_size-1] ���������� ��� ����� �� ����������� �� �������
		}
	}
};