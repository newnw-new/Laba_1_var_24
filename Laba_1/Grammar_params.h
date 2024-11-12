#pragma once
#include<vector>
#include<string>

std::vector<std::vector<std::vector<std::pair<std::string, std::string>>>> my_gram = {
	{{{"TERM", "1"}}}, //S'->Procedure (0)
	{{{"TERM", "2"}, {"TERM", "5"}, {"TERM","10"}, {"TERM", "3"}}}, //Procedure->Begin Description Operators End (1)
	{{{"KEYWORD","procedure"}, {"TERM", "4"}, {"SEP",";"}, {"KEYWORD","begin"}}}, //Begin->procedure ProcedureName ; begin (2)
	{{{"KEYWORD","end"}}}, //End->end (3)
	{{{"TERM","16"}}}, //ProcedureName->Id (4)
	{{{"KEYWORD", "var"}, {"TERM", "6"}}}, //Descriptions->var DescrList (5)
	{{{"TERM", "7"}}, 
	{{"TERM", "7"}, {"TERM", "6"}}}, //DescrList-> Descr | Descr DescrList (6)
	{{{"TERM", "8"}, {"SEP", ":"}, {"TERM", "9"}, {"SEP", ";"}}}, //Descr-> VarList : Type ; (7)
	{{{"TERM", "16"}},
	{{"TERM", "16"}, {"SEP", ";"}, {"TERM", "9"}}}, // VarList -> Id | Id , VarList (8)
	{{{"KEYWORD", "integer"}}}, // Type-> integer (9)
	{{{"TERM", "11"}},
	{{"TERM", "11"}, {"TERM", "10"}}}, // Operators -> Op | Op Operators (10)
	{{{"TERM", "16"}, {"OP", ":="}, {"TERM", "12"}, {"SEP", ";"}},
	{{"KEYWORD", "repeat"}, {"TERM", "10"}, {"KEYWORD", "until"}, {"TERM", "14"}, {"SEP", ";"}}}, // Op-> Id := Expr ; | repeat Operators until Condition ; (11)
	{{{"TERM", "13"}},
	{{"TERM", "13"}, {"OP", "+"}, {"TERM", "12"}},
	{{"TERM", "13"}, {"OP", "-"}, {"TERM", "12"}}}, // Expr-> SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr (12)
	{{{"TERM", "16"}},
	{{"TERM", "17"}},
	{{"SEP", "("}, {"TERM", "12"}, {"SEP", ")"}}}, // SimpleExpr-> Id | Const | (Expr) (13)
	{{{"TERM", "12"}, {"TERM", "15"}, {"TERM", "12"}}}, // Condition-> Expr RelationOperator Expr (14)
	{{{"OP", "="}},
	{{"OP", "<>"}},
	{{"OP", "<"}},
	{{"OP", ">"}}}, // RelationOperator-> = | <> | < | > (15)
	{{{"VAR", ""}}}, // Id -> id_name
	{{{"CONST", ""}}} // Const-> id_num
};

