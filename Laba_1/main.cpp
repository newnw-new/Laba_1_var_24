#include "Lexer.h"
#include <iostream>

int main() {
	lexer lex("Test_code.txt");
	while (!lex.read.eof()) {
		std::cout << lex.next_lexem() << '\n';
	}
}