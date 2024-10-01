#include "Lexer.h"
#include <iostream>

int main() {
	lexer lex("Test_code.txt");
	while (!lex.end()) {
		std::cout << lex.next_lexem() << '\n';
	}
}