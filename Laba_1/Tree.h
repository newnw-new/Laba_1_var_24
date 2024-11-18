#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Token_table.h"


class Node {
public:
	unsigned name;
	std::vector<Node*> sons;
public:
	Node(const unsigned name) :name{ name } {}
	Node(const unsigned name, const std::vector<Node*>& sons) :name{ name }, sons{sons}{}
	

	unsigned getName() {
		return this->name;
	}

	void add(Node* other) {
		sons.push_back(other);
	}

	std::vector<Node*> getChildren() {
		std::vector<Node*> res(sons);
		return res;
	}

	void Show(std::string tab = "") {
		for (int i = 0; i < sons.size()/2; ++i) {
			if (sons[i]->getChildren().size() == 0) {
				std::cout << tab << "	'" << token_table[sons[i]->getName()].first.getLexem() << "'\n";
			}
			else {
				std::string new_tab = tab + "	";
				sons[i]->Show(new_tab);
			}
		}
		
		std::cout << tab << name << '\n';

		for (int i = sons.size()/2; i < sons.size(); ++i) {
			if (sons[i]->getChildren().size() == 0) {
				std::cout << tab << "	'" << token_table[sons[i]->getName()].first.getLexem() << "'\n";
			}
			else {
				std::string new_tab = tab + "	";
				sons[i]->Show(new_tab);
			}
		}
	}
};