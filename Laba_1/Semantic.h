#pragma once
#include "Tree.h"
#include "string"
#include "fstream"
//#include "Symbol_table.h"


class Semantic {
	enum class states: bool {Decl = 0, Cycle = 0, Pred_F = 0};
	Node* root;
	std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar;
	std::string out_file;
	std::ofstream out;
	
public:
	Semantic(Node* root, const std::vector <std::vector
		<std::vector
		<std::pair<std::string, std::string>>>> grammar,
		const std::string out_file, const bool add_or_not = 0) : root{ root }, grammar{ grammar },
		out_file{ out_file }, out{ out_file, add_or_not ? std::ios::app : std::ios::out } {}


	
	std::vector<std::string> Create_Postfix_Write() {
		std::vector<std::string> postfix_write;


		return postfix_write;
	}

};