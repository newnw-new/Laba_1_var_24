#include "Parser.h"
#include "Grammar_params.h"

int main() {
	parser pr("Test_code.txt", "error.txt", my_gram);
	pr.algo_Erli();
}