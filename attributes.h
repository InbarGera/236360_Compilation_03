#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include <source.tab.hpp>
#include <iostream>
#include <list>
#include "output.h"

using std::cout;
using std::endl;
using std::list;
//================================= ENUMS ===============================//
enum parsRetType {
	NOTHING, INT, BYTE, BOOL, VOID, STRING	};
typedef parsRetType parsParmType;
enum varType{IS_INT, IS_ID, IS_STR};
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
/*
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
*/

//=========================== HELPER FUNCTIONS =============================//
int string_to_num(char* input){
    int sum =0;
    while(input[0] != '\0'){
        sum = sum*10 + (input++[0] - '0');
    }
    return sum;
}

char* remove_double_quotes(char* input){
    char* res = malloc(strlen(input));
    strcpy(res,input+1);
    res[strlen(res) - 1] = '\0';
    return res;
}

//=========================== ORIGINAL STYPE ===============================//
class retVal {
public:
    int integer;
    char *string;
    bool is_int;
    bool is_id;
    bool is_string;
    retVal() : integer(0), string(nullptr),
               is_int(false), is_id(false), is_string(false){};
    retVal(char* tmp_yytext, varType var_type){
        *this = retVal();
        switch (var_type){
            case IS_INT:
                integer = string_to_num(tmp_yytext);
                is_int = true;
                break;
            case IS_ID:
                string = strdup(tmp_yytext);
                is_id = true;
                break;
            case IS_STR:
                string = remove_double_quotes(tmp_yytext);
                is_string = true;
                break;
            default:
                throw;      //TODO
        }
    }

    ~retVal(){
        if(is_id || is_string)
            free(string);
    }
    retVal(const retVal& tmp){
        *this = tmp;
    }
    retVal& operator=(const retVal& tmp){
        if(this != &tmp){
            integer = tmp.integer;
            is_int = tmp.is_int;
            is_id = tmp.is_id;
            is_string = tmp.is_string;
            if(tmp.is_string || tmp.is_id)
                string = strdup(tmp.string);
            else
                string = nullptr;
        }
        return *this;
    }


};

#define YYSTYPE retVal	// Tell Bison to use STYPE as the stack type
//=============================== GRAMMAR VARIABLES CLASSES ================//
class parsedType : public retVal{
public:
    parsParmType par_type;
    parsedType(parsParmType type) : par_type(type) {};
    parsedType(const parsedType& type):
            retVal(type), par_type(type.par_type){};
    parsedType& operator=(const parsedType& type){
        return parsedType(type);
    }
};

class parsedTpID : public parsedType{
public:
    parsedTpID(): parsedType(NOTHING){};
    parsedTpID(parsedType type, retVal val) :
            retVal(val), par_type(type.par_type){};
    parsedTpID(const parsedTpID& type_id): parsedType(type_id){};
    parsedTpID& operator=(const parsedTpID& type_id){
        return parsedTpID(type_id);
    }
};

class parsedNum : public retVal{
public:
    parsParmType par_type;
    parsedNum(){
        par_type = NOTHING;

    }
    parsedNum(parsParmType type) : par_type(type){};
    parsedNum(retVal val, parsedNum type_num): retVal(val){
        if(type_num.par_type == NOTHING)
            par_type = INT;
        else
            par_type = BYTE;
    };
    parsedNum(const parsedNum& num) : retVal(num), par_type (num.par_type){};
    parsedNum& operator=(const parsedNum& num){
        return parsedNum(num);

    }
};

class parsedStatement : public retVal{
public:

};

class parsedOp: public retVal {
    binOps charOpToEnum(char* opToEdit){
        switch(strlen(opToEdit)) {
            case 0:
                throw;      //TODO
            case 1:
                switch (opToEdit[0]) {
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
            case 2:
                switch (opToEdit[0]) {
                    case '&':
                        if(opToEdit[1] != '&')
                            throw;  //TODO
                        return AND;
                    case '|':
                        if(opToEdit[1] != '|')
                            throw;  //TODO
                        return OR;
                    case '=':
                        if(opToEdit[1] != '=')
                            throw;  //TODO
                        return EQ;
                    case '!':
                        if(opToEdit[1] != '=')
                            throw;  //TODO
                        return NEQ;
                    case '>':
                        if(opToEdit[1] != '=')
                            throw;  //TODO
                        return GEQ;
                    case '<':
                        if(opToEdit[1] != '=')
                            throw;  //TODO
                        return LEQ;
                    default:
                        throw;  //TODO
                }
            default:
                throw;      //TODO
        }
    }

public:
    binOps op;
    parsedOp(){
        op = NO_OP;
    }
    parsedOp(char* op_to_parse){
        op = charOpToEnum(op_to_parse);
    }
    parsedExp makeOp(parsedExp exp1,parsedExp second_exp){
        parsedExp& a = exp1;
        parsedExp& b = second_exp;
        switch (op){
            case AND:
                    return (a&&b);
            case OR:
                    return (a||b);
            case EQ:
                    return (a==b);
            case NEQ:
                    return (a!=b);
            case LT:
                    return (a<b);
            case GT:
                    return (a>b);
            case LEQ:
                    return (a<=b);
            case GEQ:
                    return (a>=b);
            case PLUS:
                    return (a+b);
            case MINUS:
                    return (a-b);
            case MUL:
                    return (a*b);
            case DIV:
                    return (a/b);
        }
    }
};

class parsedExp : public parsedNum{
public:
    parsedExp(){};
    parsedExp(parsedNum num) : parsedNum(num){};
    parsedExp(bool b){
        par_type = BOOL;
        if(b)
            integer = 1;
        else
            integer = 0;
    }
    parsedExp(retVal val): parsedNum(val, NOTHING){
        if(is_string)
            par_type = STRING;
    }
    parsedExp(parsedExp exp1, parsedOp op, parsedExp exp2){
        *this= op.makeOp(exp1,exp2);
    }
    parsedExp(const parsedExp& exp) : parsedNum(exp){};
    parsedExp& operator=(const parsedExp& exp){
        return parsedExp(exp);
    }

    //operators for the expression - I know it's a heck of a code duplication....
    parsedExp operator==(parsedExp exp2){
        if(!is_int || !exp2.is_int )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.is_int = true;
        new_exp.integer = (integer == exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator!=(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer != exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator>=(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer >= exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator<=(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer <= exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator<(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer < exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator>(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer > exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator+(parsedExp exp2){
        if(!is_int || exp2 != is_int)
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.is_int = true;
        new_exp.integer = (integer + exp2.integer);
        return new_exp;
    };
    parsedExp operator-(parsedExp exp2){
        parsedExp new_exp = (*this+exp2);
        new_exp.integer = (integer - exp2.integer);
        return new_exp;
    };
    parsedExp operator*(parsedExp exp2){
        parsedExp new_exp = (*this+exp2);
        new_exp.integer = (integer * exp2.integer);
        return new_exp;
    };
    parsedExp operator/(parsedExp exp2){
        parsedExp new_exp = (*this+exp2);
        new_exp.integer = (integer / exp2.integer);
        return new_exp;
    };
    parsedExp operator&&(parsedExp exp2){
        if((par_type != BOOL )||(exp2.par_type != BOOL) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.integer = (integer && exp2.integer);
        return new_exp;
    };
    parsedExp operator||(parsedExp exp2){
        parsedExp new_exp = (*this && exp2);
        new_exp.integer = (integer || exp2.integer);
        return new_exp;
    };
    parsedExp operator!(){
        if (par_type != BOOL)
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.integer = 1-integer;
        return new_exp;
    };

};


#endif