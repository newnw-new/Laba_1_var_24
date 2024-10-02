#include "Lexer.h"
#include <iostream>
#include "Token.h"
#include <string>

int main() {
	lexer lex("Test_code.txt");
	while (!lex.end()) {
		std::string lexem = lex.next_lexem();
		std::cout << token(lexem) << '\n';
	}
}