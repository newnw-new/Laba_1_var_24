#pragma once

class situation
{
public:
	unsigned rule = 0;
	unsigned sub_rule = 0;
	unsigned dot = 0;
	unsigned sym_index = 0;

	situation(unsigned rule, unsigned sub_rule, unsigned dot, unsigned sym_index) : rule{ rule }, sub_rule{ sub_rule }, dot{ dot }, sym_index{ sym_index } {}
	situation() = default;

	bool operator == (const situation& other) const {
		return (this->rule == other.rule) && (this->sub_rule == other.sub_rule)
			&& (this->dot == other.dot) && (this->sym_index == other.sym_index);
	}

	bool operator != (const situation& other) const {
		return !(*this == other);
	}

	bool operator > (const situation& other) const {
		if (this->rule != other.rule) {
			return this->rule > other.rule;
		}
		else {
			return this->sub_rule > other.rule;
		}
	}

	bool operator >= (const situation& other) const {
		return (*this) > other || (*this == other);
	}

	bool operator < (const situation& other) const {
		return !((*this > other) || (*this == other));
	}

	bool operator <= (const situation& other) const {
		return !(*this > other) || (*this == other);
	}
};

bool comp_sit(const situation& a, const situation& b) {
	return b > a;
}