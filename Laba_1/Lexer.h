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
	dfa lex_dfa{ 6, alp, fin_states_lex, trans_func_lex };
	enum class states {Start, General, Oper};
	char symbol; // ������� ������ �� ������� ����������� ����������� ����������

public:
	lexer(const std::string& file_name, const std::string& output_name = "error.txt") :file{ file_name }, output_file{ output_name } {
		read.open(file);
		if (!read.eof())
			read.get(symbol);
		else
			throw "File is Empty!";
		out.open(output_file);
	};
	lexer() = delete;
	//���������� ��������� �������
	std::string next_lexem() {
		std::string lexem;
		bool flag = true;
		states state = states::Start;

		while (!read.eof() && flag) {
			switch (state) {
			case states::Start:
				if (symbol == '<' || symbol == ':') {
					lexem += symbol;
					state = states::Oper; 
				}
				else if (symbol == '+' || symbol == '-' || symbol == '(' || symbol == ')'
					|| symbol == ',' || symbol == ';' || symbol == '=' || symbol == '>') {
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
					state = states::Start;
					flag = false;
				}
				else {
					lexem += symbol;
				}
				break;
			case states::Oper:
				if ((lexem == "<" && symbol == '>') || (lexem == ":" && symbol == '=')) {
					lexem += symbol;
					state = states::Start;
					read.get(symbol);
					flag = false;
				}
				else {
					state = states::Start;
					flag = false;
				}
				break;
			}
			if(flag)
				read.get(symbol);
		}

		try {
			if (!lex_dfa.isAccept(lexem)) {
				out << "undefined lexem \"" << lexem << "\" on line : " << num_line << '\n';
			}
		}
		catch (...) {
			out << "undefined lexem \"" << lexem << "\" on line : " << num_line << '\n';
		}

		return lexem;
	}

	bool end() {
		return read.eof();
	}
};