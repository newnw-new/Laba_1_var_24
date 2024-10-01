#pragma once
#include <string>
#include <fstream>

class lexer {
	std::string file;
	int num_str = 0;
	std::ifstream read;
	enum class states {Start, General, Oper1, Oper2};
	states state = states::Start;
	char symbol; // текущий символ на котором остановился лексический анализатор

public:
	lexer(const std::string& file_name) :file{ file_name }, read{ file_name } {
		read >> symbol;
	};
	lexer() = delete;

	//Возвращает следующую лексему
	std::string next_lexem() {
		std::string lexem;
		while (true) {
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
					return lexem;
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
					return lexem;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					return lexem;
				}
				else {
					lexem += symbol;
				}
				break;
			case states::Oper1:
				if (symbol == '>' || symbol == '=') {
					lexem += symbol;
					state = states::Start;
					return lexem;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					return lexem;
				}
				else {
					state = states::Start;
					return lexem;
				}
				break;
			case states::Oper2:
				if (symbol == '=') {
					lexem += symbol;
					state = states::Start;
					return lexem;
				}
				else if (symbol == '\n') {
					num_str++;
					state = states::Start;
					return lexem;
				}
				else {
					state = states::Start;
					return lexem;
				}
				break;
			}
		}
		read >> symbol;
	}
};