#pragma once
#include <vector>
#include <string>
#include <iostream>


class Node {
public:
	std::string name;
	std::vector<Node*> sons;
public:
	Node(const std::string name) :name{ name } {}
	Node(const std::string name, const std::vector<Node*>& sons) :name{ name }, sons{sons}{}
	

	std::string getName() {
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
		for (int i = 0; i <= sons.size()/2; ++i) {
			if (sons[i]->getChildren().size() == 0) {
				std::cout << tab << "	" << sons[i]->getName() << '\n';
			}
			else {
				std::string new_tab = tab + "	";
				sons[i]->Show(new_tab);
			}
		}
		
		std::cout << tab << name << '\n';

		for (int i = sons.size()/2+1; i < sons.size(); ++i) {
			if (sons[i]->getChildren().size() == 0) {
				std::cout << tab << "	" << sons[i]->getName() << '\n';
			}
			else {
				std::string new_tab = tab + "	";
				sons[i]->Show(new_tab);
			}
		}
	}
};