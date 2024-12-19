#pragma once
#include "Tree.h"
#include "string"
#include "fstream"
#include <stack>
#include <map>
#include "Token_table.h"
//#include "SymbolTable.h"


class Semantic {
	std::map<std::string, bool> symbol_table;
	std::map<std::string, bool> states = { std::pair<std::string, bool>{"Decl", 0},
	std::pair<std::string, bool>{"Decl_F", 0},
	std::pair<std::string, bool>{"Condition", 0}, };
	Node* root;
	std::string out_file;
	std::ofstream out;
	unsigned counter = 0;
	long long m_num = -2;
	std::stack<std::string> st;
	std::stack<unsigned> cyrcles;
	
public:
	Semantic(Node* root, const std::string out_file, const bool add_or_not = 0) : root{ root },
		out_file{ out_file }, out{ out_file, add_or_not ? std::ios::app : std::ios::out } {}


private:
	void Bypass_Tree(Node* root, std::vector<std::string>& postfix_write, bool &correct) {

		if (root->getChildren().size() != 0) {
			if (root->getName() == 5) { states["Decl"] = 1; }
			for (long long i = root->getChildren().size()-1; i >=0 ; --i) {
				Bypass_Tree(root->getChildren()[i], postfix_write, correct);
				if (!correct) {
					return;
				}
			}
			if (root->getName() == 5) { states["Decl"] = 0; }
		}
		else {
			token leef = token_table[root->getName()].first;
			if (leef.getLexem() == "procedure") {
				states["Decl_F"] = 1;
			}
			else if (leef.getType() == "VAR") {
				if (states["Decl_F"] == 1) {
					postfix_write.push_back(leef.getLexem());
					postfix_write.push_back("DECL_F\n");
					states["Decl_F"] = 0;
				}
				else if (states["Decl"] == 1) {
					if (symbol_table.find(leef.getLexem()) == symbol_table.end()) {
						symbol_table.insert(std::pair<std::string, bool>{leef.getLexem(), false});
						postfix_write.push_back(leef.getLexem());
						counter++;
					}
					else {
						out << "an re-declared variable " << leef.getLexem() << " on line "
							<< token_table[root->getName()].second << '\n';
						correct = false;
						postfix_write.clear();
					}
				}
				else {
					if (symbol_table.find(leef.getLexem()) == symbol_table.end()) {
						out << "an uncreated variable " << leef.getLexem() << " is used on line "
							<< token_table[root->getName()].second << '\n';
						correct = false;
						postfix_write.clear();
					}
					else if (states["Condition"] == 1 && symbol_table[leef.getLexem()] == false) {
						out << "an uninitialized variable " << leef.getLexem() << " is used on line "
							<< token_table[root->getName()].second << '\n';
						correct = false;
						postfix_write.clear();
					}
					else {
						postfix_write.push_back(leef.getLexem());
						states["Condition"] = 1;
					}
				}
			}
			else if (leef.getLexem() == "integer") {
				postfix_write.push_back(leef.getLexem());
				postfix_write.push_back(std::to_string(counter+1));
				postfix_write.push_back("DECL\n");
				counter = 0;
			}
			else if (leef.getType() == "CONST") {
				postfix_write.push_back(leef.getLexem());
			}
			else if (leef.getType() == "OP") {
				if (leef.getLexem() == ":=") {
					st.push(postfix_write[postfix_write.size() - 1]);
					st.push(":=");
				}
				else if (leef.getLexem() == "+" || leef.getLexem() == "-") {
					if (!st.empty() && st.top() == "+" || st.top() == "-")
					{
						postfix_write.push_back(st.top());
						st.pop();
					}
					st.push(leef.getLexem());
				}
				else if (leef.getLexem() == "<" || leef.getLexem() == ">"
					|| leef.getLexem() == "=" || leef.getLexem() == "<>") {
					while (!st.empty()) {
						postfix_write.push_back(st.top());
					}
					st.push(leef.getLexem());
				}
			}
			else if (leef.getType() == "SEP") {
				if (leef.getLexem() == "(") {
					st.push("(");
				}
				else if (leef.getLexem() == ")") {
					while (!st.empty() && st.top() != "(") {
						postfix_write.push_back(st.top());
						st.pop();
					}
					if (!st.empty()) {
						st.pop();
					}
				}
				else if (leef.getLexem() == ";") {
					while (!st.empty() && st.top() != ":=" && st.top() != "<" 
						&& st.top() != ">" && st.top() != "<>" && st.top() != "=") {
						postfix_write.push_back(st.top());
						st.pop();
					}
					if (!st.empty() && st.top() == ":=") {
						postfix_write.push_back(st.top()+"\n");
						st.pop();
						symbol_table[st.top()] = true;
						st.pop();
						states["Condition"] = 0;
					}
					else if(!st.empty()){
						postfix_write.push_back(st.top());
						st.pop();
						states["Condition"] = 0;
						postfix_write.push_back("m" + std::to_string(cyrcles.top()+1));
						postfix_write.push_back("BF");
						postfix_write.push_back("m" + std::to_string(cyrcles.top()));
						postfix_write.push_back("BRL");
						postfix_write.push_back("m" + std::to_string(cyrcles.top()+1));
						postfix_write.push_back("DEFL\n");
						cyrcles.pop();
					}
				}
			}
			else if (leef.getLexem() == "repeat") {
				m_num += 2;
				cyrcles.push(m_num);
				postfix_write.push_back("m" + std::to_string(cyrcles.top()));
				postfix_write.push_back("DEFL");
			}
			else if (leef.getLexem() == "until") {
				states["Condition"] = 1;
			}
			else if (leef.getLexem() == "begin") {
				postfix_write.push_back("FBEGIN\n");
			}
			else if (leef.getLexem() == "end") {
				postfix_write.push_back("FEND\n");
			}
		}
	}

public:
	std::vector<std::string> Create_Postfix_Write() {
		std::vector<std::string> postfix_write;
		bool correct = true;
		Bypass_Tree(root, postfix_write, correct);
		return postfix_write;
	}

};