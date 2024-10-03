#include "Lexer.h"
#include <iostream>
#include "Token.h"
#include <string>
#include "hashTable.h"
#include <iomanip>

int main() {
	lexer lex("Test_code.txt");
	HashTable table;
	while (!lex.end()) {
		token t(lex.next_lexem());
		table.add(t);
		std::cout << std::setw(5) << table.find(t) << " |" << t << '\n';
	}
}