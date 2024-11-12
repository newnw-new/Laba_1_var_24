#pragma once

struct situation
{
	unsigned rule = 0;
	unsigned sub_rule = 0;
	unsigned dot = 0;
	unsigned sym_index = 0;

	situation(unsigned rule, unsigned sub_rule, unsigned dot, unsigned sym_index) : rule{ rule }, sub_rule{ sub_rule }, dot { dot }, sym_index{ sym_index } {}
	situation() = default;
};