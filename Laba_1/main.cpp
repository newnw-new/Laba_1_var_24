#include "Lexer.h"
#include <iostream>
#include "Token.h"
#include <string>
//#include "hashTable.h"
//#include <iomanip>
//#include <fstream>
#include "Parser.h"
#include "Grammar_params.h"

int main() {
	//std::ofstream out("output.txt");
	parser pr("Test_code.txt", "error.txt", my_gram);
	pr.algo_Erli();
	std::cout << (situation(6, 1, 0, 0) == situation(6, 1, 0, 0));

	/*lexer lex("Test_code.txt", "error.txt");
	HashTable table;
	while (!lex.end()) {
		token t(lex.next_lexem());
		if (t.getType() != "UNDEFINED") {
			table.add(t);
			out << std::setw(5) << table.find(t) << " |" << t << '\n';
		}
	}*/
}