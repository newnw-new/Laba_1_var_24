#pragma once
#include <string>
#include <fstream>
#include "dfa.h"
#include "dfa_parametrs.h"

class lexer {
	std::string file;
	std::ifstream read;
	std::string output_file;
	std::ofstream out;
	int num_line = 1;
	dfa lex_dfa{ 7, alp, fin_states_lex, trans_func_lex };
	enum class states {Start, General, Oper1, Oper2};
	states state = states::Start;
	char symbol; // ������� ������ �� ������� ����������� ����������� ����������
	bool true_end = false;

public:
	lexer(const std::string& file_name, const std::string& output_name = "output.txt") :file{ file_name }, output_file{ output_name } {
		read.open(file);
		if(!read.eof())
			read.get(symbol);
		out.open(output_file);
	};
	lexer() = delete;
	//���������� ��������� �������
	std::string next_lexem() {
		std::string lexem;
		bool flag = true;
		bool next_line = false;
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
				else if (symbol == '\n') { num_line++; }
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
					next_line = true;
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
					next_line = true;
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
					next_line = true;
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
		if (read.eof() && (lexem.size() > 0 && symbol == lexem[lexem.size()-1])
			|| (lexem.size() == 0 && !true_end)) {
			true_end = true;
			if(lexem.size() == 0)
				lexem = symbol;
		}
		try {
			if (!lex_dfa.isAccept(lexem)) {
				out << "undefined lexem \"" << lexem << "\" on line : " << num_line << '\n';
			}
		}
		catch (...) {
			out << "undefined lexem \"" << lexem << "\" on line : " << num_line << '\n';
		}
		if (next_line)
			num_line++;
		return lexem;
	}

	bool end() {
		return true_end;
	}
};