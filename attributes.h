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

class parsedNum : retVal{
public:
    parsParmType par_type;
    int parm_val;

};

class parsedStatement : retVal{

};

class parsedOp: retVal {
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

class parsedExp : retVal{
public:
    parsParmType par_type;
    int parm_val;
    parsedExp(){
        par_type = NOTHING;
        parm_val = -1;      //WHAT SHOULD IT BE??? TODO
    };
    parsedExp(bool b){
        par_type = BOOL;
        if(b)
            parm_val = 1;
        else
            parm_val = 0;
    }
    parsedExp(char* str){

    }   //TODO
    parsedExp(parsedExp exp1, parsedOp op, parsedExp exp2){
        return op.makeOp(exp1,exp2);
    }


    //operators for the expression - I know it's a heck of a code duplication....
    parsedExp operator==(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
                (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val == exp2.parm_val);
        return new_exp;
    };
    parsedExp operator!=(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val != exp2.parm_val);
        return new_exp;
    };
    parsedExp operator>=(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val >= exp2.parm_val);
        return new_exp;
    };
    parsedExp operator<=(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val <= exp2.parm_val);
        return new_exp;
    };
    parsedExp operator<(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val < exp2.parm_val);
        return new_exp;
    };
    parsedExp operator>(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val > exp2.parm_val);
        return new_exp;
    };
    parsedExp operator+(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.parm_val = (parm_val + exp2.parm_val);
        return new_exp;
    };
    parsedExp operator-(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.parm_val = (parm_val - exp2.parm_val);
        return new_exp;
    };
    parsedExp operator*(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.parm_val = (parm_val * exp2.parm_val);
        return new_exp;
    };
    parsedExp operator/(parsedExp exp2){
        if((par_type != INT && par_type != BYTE)||
           (exp2.par_type != INT && exp2.par_type != BYTE) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.parm_val = (parm_val / exp2.parm_val);
        return new_exp;
    };
    parsedExp operator&&(parsedExp exp2){
        if((par_type != BOOL )||(exp2.par_type != BOOL) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val && exp2.parm_val);
        return new_exp;
    };
    parsedExp operator||(parsedExp exp2){
        if((par_type != BOOL )||(exp2.par_type != BOOL) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = (parm_val || exp2.parm_val);
        return new_exp;
    };
    parsedExp operator!(){
        if (par_type != BOOL)
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.parm_val = 1-parm_val;
        return new_exp;
    };

};

class retVal {
    int integer;
    char *string;

    retVal() : integer(0), string(NULL) {};

};

#define YYSTYPE retVal	// Tell Bison to use STYPE as the stack type

#endif