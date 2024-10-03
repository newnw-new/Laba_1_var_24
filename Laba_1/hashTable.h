#pragma once
#include "Token.h"
#include <string>
#include <cmath>

class HashTable{
private:
	token* massive;
	long long size;

	long long HashFunc(const token& t) const {
		long long res = 0;
		std::string str_res;
		std::string lexem = t.getLexem();
		for (long long i = 0; i < lexem.size(); ++i) {
			str_res += std::to_string(int(lexem[i]));
			res += stoi(str_res) % size;
			str_res = std::to_string(res);
		}
		return res%size;
	}

	void re_create() {
		size *= 2;
		token* new_massive = new token[size];
		for (int i = 0; i < size / 2; ++i) {
			if (massive[i] != token()) {
				long long ind = HashFunc(massive[i]);
				while (new_massive[ind] != token()) {
					ind = (ind + 1) % size;
				}
				new_massive[ind] = massive[i];
			}
		}
		delete[] massive;
		massive = new_massive;
	}

public:
	HashTable(const long long& size) :size{ size } {
		massive = new token[this->size];
	}

	HashTable() :size{ 10000 } {
		massive = new token[10000];
	}

	~HashTable() {
		delete[] massive;
	}

	void add(const token& t) {
		if (this->find(t) == -1) {
			long long ind = HashFunc(t);
			while (massive[ind] != token()) {
				ind = (ind + 1) % size;
				if (ind == HashFunc(t)) {
					re_create();
					add(t);
					return;
				}
			}
			massive[ind] = t;
		}
	}

	long long find(const token& t) const {
		unsigned long long ind = HashFunc(t);
		while (true) {
			if (massive[ind] != t) {
				ind = (ind + 1) % size;
			}
			else {
				break;
			}
			if (ind == HashFunc(t)) {
				return -1;
			}
		}
		return ind;
	}
};