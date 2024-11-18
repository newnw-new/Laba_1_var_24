#pragma once
#include<vector>
#include<string>

std::vector<std::vector<std::vector<std::pair<std::string, std::string>>>> my_gram = {
	{{{"N", "1"}}}, //S'->Procedure (0)
	{{{"N", "2"}, {"N", "5"}, {"N","10"}, {"N", "3"}}}, //Procedure->Begin Description Operators End (1)
	{{{"KEYWORD","procedure"}, {"N", "4"}, {"SEP",";"}, {"KEYWORD","begin"}}}, //Begin->procedure ProcedureName ; begin (2)
	{{{"KEYWORD","end"}}}, //End->end (3)
	{{{"N","16"}}}, //ProcedureName->Id (4)
	{{{"KEYWORD", "var"}, {"N", "6"}}}, //Descriptions->var DescrList (5)
	{{{"N", "7"}}, 
	{{"N", "7"}, {"N", "6"}}}, //DescrList-> Descr | Descr DescrList (6)
	{{{"N", "8"}, {"SEP", ":"}, {"N", "9"}, {"SEP", ";"}}}, //Descr-> VarList : Type ; (7)
	{{{"N", "16"}},
	{{"N", "16"}, {"SEP", ","}, {"N", "8"}}}, // VarList -> Id | Id , VarList (8)
	{{{"KEYWORD", "integer"}}}, // Type-> integer (9)
	{{{"N", "11"}},
	{{"N", "11"}, {"N", "10"}}}, // Operators -> Op | Op Operators (10)
	{{{"N", "16"}, {"OP", ":="}, {"N", "12"}, {"SEP", ";"}},
	{{"KEYWORD", "repeat"}, {"N", "10"}, {"KEYWORD", "until"}, {"N", "14"}, {"SEP", ";"}}}, // Op-> Id := Expr ; | repeat Operators until Condition ; (11)
	{{{"N", "13"}},
	{{"N", "13"}, {"OP", "+"}, {"N", "12"}},
	{{"N", "13"}, {"OP", "-"}, {"N", "12"}}}, // Expr-> SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr (12)
	{{{"N", "16"}},
	{{"N", "17"}},
	{{"SEP", "("}, {"N", "12"}, {"SEP", ")"}}}, // SimpleExpr-> Id | Const | (Expr) (13)
	{{{"N", "12"}, {"N", "15"}, {"N", "12"}}}, // Condition-> Expr RelationOperator Expr (14)
	{{{"OP", "="}},
	{{"OP", "<>"}},
	{{"OP", "<"}},
	{{"OP", ">"}}}, // RelationOperator-> = | <> | < | > (15)
	{{{"VAR", "value"}}}, // Id -> id_name (16)
	{{{"CONST", "const"}}} // Const-> id_num (17)
};

