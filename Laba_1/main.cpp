#include "Lexer.h"
#include <iostream>
#include "Token.h"
#include <string>
#include "hashTable.h"
#include <iomanip>
#include <fstream>

int main() {
	std::ofstream out("output.txt");
	lexer lex("Test_code.txt", "error.txt");
	HashTable table;
	while (!lex.end()) {
		token t(lex.next_lexem());
		if (t.getType() != "UNDEFINED") {
			table.add(t);
			out << std::setw(5) << table.find(t) << " |" << t << '\n';
		}
	}
}