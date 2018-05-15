#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <iostream>

using std::cout;
using std::endl;

enum parsRetType {
	NOTHING, INT, BYTE, BOOL, VOID, STRING	};

typedef parsRetType parsParmType;

enum binOps {		//binary operators - both relative and arithmatical
	NO_OP,	//not an operation
	AND,	//and				&&
	OR,		//					||
	EQ,		//equal				==
	NEQ,	//not equal			!=
	LT,		//lesser than		<
	GT,		//greater than		>
	LEQ,	//lesser or equal	<=
	GEQ,	//greater or equal	>=
	PLUS,	//plus				+
	MINUS,	//minus				-
	MUL,	//multiply			*
	DIV		//divide			/

};

class parseInfo : retVal
{
private:
	binOps charOpToEnum(char* opToEdit){
		if(strlen(opToEdit) == 1){
			switch (opToEdit[0]){
				case '<':
					return LT;
				case '>':
					return GT;
				case '+':
					return PLUS;
				case '-':
					return MINUS;
				case '*':
					return MUL;
				case '/':
					return DIV;
				default:
					cout << "Error! not a valid operation!" << endl;
					return NO_OP;
			}
		}else{	//strlen(opToEdit) = 2
			switch (opToEdit[0]){
				case '&':
					return AND;
				case '|':
					return OR;
				case '=':
					return EQ;
				case '!':
					return NEQ;
				case '>':
					return GEQ;
				case '<':
					return LEQ;
				default:
					cout << "Error! not a valid operation!" << endl;
					return NO_OP;
			}
		}
	}
	bool checkNumOrBoolTypes(parsParmType type1, parsParmType type2, bool is_bool){
		if(is_bool){
			if((type1 != type2)|| type1!= BOOL)
				return false;
		}
		if((type1 != INT && type1 != BYTE) || (type2 != INT && type2 != BYTE))
			return false;
		return true;
	}
public:
	int ret_val;
	parsRetType ret_type;	//used for functions return value
	int parm_val;
	parsParmType parm_type;	//used for parameters
	bool is_while_scope;
	parsParmType checkBinOpParmType(parseInfo exp){
		if(parm_type == INT || exp.parm_type == INT)
			return INT;
		return BYTE;
	}
	int makeOp(char* op_to_make,parseInfo second_exp){
		binOps operation = charOpToEnum(op_to_make);
		a = parm_val;
		b = second_exp.parm_val;
		switch (operation){
			case AND:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, true))
					return (a&&b);
				yyerror();	//TODO
			case OR:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, true))
					return (a||b);
				yyerror();	//TODO
			case EQ:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
						return (a==b);
				yyerror();	//TODO
			case NEQ:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a!=b);
				yyerror();	//TODO
			case LT:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a<b);
				yyerror();	//TODO
			case GT:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a>b);
				yyerror();	//TODO
			case LEQ:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a<=b);
				yyerror();	//TODO
			case GEQ:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a>=b);
				yyerror();	//TODO
			case PLUS:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a+b);
				yyerror();	//TODO
			case MINUS:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a-b);
				yyerror();	//TODO
			case MUL:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a*b);
				yyerror();	//TODO
			case DIV:
				if(checkNumOrBoolTypes(parm_type, second_exp.parm_type, false))
					return (a/b);
				yyerror();	//TODO
		}
	}
};




typedef union
{
	constAttribute constVal;
	expAttribute exp;
} STYPE;

#define YYSTYPE STYPE	// Tell Bison to use STYPE as the stack type

#endif