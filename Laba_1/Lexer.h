#pragma once
#include <string>

class lexer {
	std::string file;
	int num_str = 0;

public:
	lexer(const std::string& file_name) :file{ file_name } {};
	lexer() = delete;

	//���������� ��������� �������
	std::string next_lexem() {

	}
};