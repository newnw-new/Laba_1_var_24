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
				grammar[i.rule][i.sub_rule][i.dot].first != "TERM") // Если точка не стоит в конце ситуации и точка стоит перед терминалом
			{
				if ((grammar[i.rule][i.sub_rule][i.dot] == lexem) ||
					((grammar[i.rule][i.sub_rule][i.dot].first == "VAR" && lexem.getType() == "VAR") ||
						(grammar[i.rule][i.sub_rule][i.dot].first == "CONTS" && lexem.getType() == "CONTS"))) { // Если терминал равен лексеме то вставляем его в множество ситуаций D[D_size-1]
					D[D_size - 1].insert(situation(i.rule, i.sub_rule, i.dot + 1, i.sym_index));
				}
			}
		}
		if (S_size == D[D_size - 1].size()) { // Если размер множества ситуаций не изменился значит входящая лексема не подошла ни одной ситуации из предыдущего множества
			// Здесь нужно сообшить об ошибке и переместить все ситуации в D[D_size-1] подходящие для скана не оглядываясь на лексему
		}
	}
};