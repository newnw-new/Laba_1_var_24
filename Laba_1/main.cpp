#include "Parser.h"
#include "Grammar_params.h"
#include "Tree.h"

int main() {
	parser pr("Test_code.txt", "error.txt", my_gram);
	Node* root = pr.buildTree(pr.algo_Erli());
	root->Show();
	int a = 3;
}