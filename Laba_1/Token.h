#pragma once
#include <string>
#include "dfa.h"
#include "dfa_parametrs.h"
#include <iostream>
#include <iomanip>

class token {
	std::string type;
	std::string lexem;

public:
	token(std::string lexem): lexem { lexem }{
		try {
			if (this->lexem == "integer" || this->lexem == "procedure" || this->lexem == "begin"
				|| this->lexem == "end" || this->lexem == "var" || this->lexem == "until"
				|| this->lexem == "repeat") {
				type = "KEYWORD";
			}
			else if (dfa(2, alp, fin_states_CONST, trans_func_CONST).isAccept(this->lexem)) {
				type = "CONST";
			}
			else if (dfa(2, alp, fin_states_VAR, trans_func_VAR).isAccept(this->lexem)) {
				type = "VAR";
			}
			else if (dfa(4, alp, fin_states_OP, trans_func_OP).isAccept(this->lexem)) {
				type = "OP";
			}
			else if (dfa(2, alp, fin_states_SEP, trans_func_SEP).isAccept(this->lexem)) {
				type = "SEP";
			}
			else {
				type = "UNDEFINED";
			}
		}
		catch (...) {
			type = "UNDEFINED";
		}
	}

	token() = default;

	std::string getType() const {
		return type;
	}

	std::string getLexem() const {
		return lexem;
	}

	bool operator == (const token& other_t) const{
		return (other_t.type == this->type && other_t.lexem == this->lexem);
	}

	bool operator != (const token& other_t) const{
		return !(*this == other_t);
	}
	
	friend bool operator == (const std::pair<std::string, std::string>& other_t, const token& t);
	friend bool operator == (const token& t, const std::pair<std::string, std::string>& other_t);
	friend bool operator != (const std::pair<std::string, std::string>& other_t, const token& t);
	friend bool operator != (const token& t, const std::pair<std::string, std::string>& other_t);
	friend std::ostream& operator <<(std::ostream& os, const token& t);
};

bool operator == (const std::pair<std::string, std::string>& other_t, const token& t) {
	return (other_t.first == t.type && other_t.second == t.lexem);
}

bool operator == (const token& t, const std::pair<std::string, std::string>& other_t) {
	return other_t == t;
}

bool operator != (const std::pair<std::string, std::string>& other_t, const token& t) {
	return !(other_t == t);
}

bool operator != (const token& t, const std::pair<std::string, std::string>& other_t) {
	return !(other_t == t);
}

std::ostream& operator << (std::ostream& os, const token& t) {
	return os << std::setw(20) << t.type << " |" << std::setw(20) << t.lexem << " |";
}