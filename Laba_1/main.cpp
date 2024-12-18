#include "Parser.h"
#include "Grammar_params.h"
#include "Tree.h"
#include "Semantic.h"
#include "iostream"

int main() {
	parser pr("Test_code.txt", "error.txt", my_gram);
	Node* root = pr.buildTree();
	if (root != nullptr) {
		Semantic sm(root, my_gram, "error.txt", true);
		std::vector<std::string> vec = sm.Create_Postfix_Write();
		for (unsigned i = 0; i < vec.size(); ++i) {
			std::cout << vec[i] << " ";
		}
	}
}