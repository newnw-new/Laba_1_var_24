#pragma once
#include <string>
#include <fstream>
#include "dfa.h"
#include "dfa_parametrs.h"

class lexer {
	std::string file;
	std::ifstream read;
	std::ofstream out;
	int num_str = 1;
	dfa lex_dfa{ 7, alp, fin_states_lex, trans_func_lex };
	enum class states {Start, General, Oper1, Oper2};
	states state = states::Start;
	char symbol; // текущий символ на котором остановился лексический анализатор

public:
	lexer(const std::string& file_name) :file{ file_name } {
		read.open(file);
		read.get(symbol);
		out.open("output.txt");
	};
	lexer() = delete;
	//Возвращает следующую лексему
	std::string next_lexem() {
		std::string lexem;
		bool flag = true;
		while (!read.eof() && flag) {
			switch (state) {
			case states::Start:
				if (symbol == '<') { 
					lexem += symbol;
					state = states::Oper1; 
				}
				else if (symbol == '>' || symbol == ':') {
					lexem += symbol;
					state = states::Oper2; 
				}
				else if (symbol == '+' || symbol == '-' || symbol == '(' || symbol == ')'
					|| symbol == ',' || symbol == ';' || symbol == '=') {
					lexem += symbol;
					read.get(symbol);
					flag = false;
				}
				else if (symbol == '\n') { num_str++; }
				else if (symbol != ' ' && symbol != '\t') {
					lexem += symbol;
					state = states::General;
				}
				break;
			case states::General:
				if (symbol == ' ' || symbol == ';' || symbol == '+'
					|| symbol == '-' || symbol == ',' || symbol == '\t'
					|| symbol == '(' || symbol == ')' || symbol == '>'
					|| symbol == '<' || symbol == ':' || symbol == '=') {
					state = states::Start;
					flag = false;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					flag = false;
				}
				else {
					lexem += symbol;
				}
				break;
			case states::Oper1:
				if (symbol == '>' || symbol == '=') {
					lexem += symbol;
					state = states::Start;
					read.get(symbol);
					flag = false;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					flag = false;
				}
				else {
					state = states::Start;
					flag = false;
				}
				break;
			case states::Oper2:
				if (symbol == '=') {
					lexem += symbol;
					state = states::Start;
					read.get(symbol);
					flag = false;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					flag = false;
				}
				else {
					state = states::Start;
					flag = false;
				}
				break;
			}
			read.get(symbol);
		}
		try {
			if (!lex_dfa.isAccept(lexem)) {
				out << "undefined lexem \"" << lexem << "\" on line : " << num_str << '\n';
			}
		}
		catch (...) {
			out << "undefined lexem \"" << lexem << "\" on line : " << num_str << '\n';
		}
		return lexem;
	}

	bool end() {
		return read.eof();
	}
};