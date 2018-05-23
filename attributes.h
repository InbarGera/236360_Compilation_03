#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include "parser.tab.hpp"
#include <iostream>
#include <list>
#include <string>
#include "output.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
//================================= ENUMS ===============================//
enum parsRetType {
    NOTHING, INT, BYTE, BOOL, VOID, STRING, STRUCT	};
typedef parsRetType parsParmType;
enum GrammerVar{
    NOT_INITIALIZED,
    IS_BOOL,
    IS_INT,
    IS_ID,
    IS_STR,
    IS_CALL,
    IS_ARRAY
};
/*
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
 */
enum binOps {
    REL_OP,
    BOOL_OP,
    MATH_OP
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
enum typeKind {VOID, BOOL, INTEGER, BYTE, STRING, ARRAY};

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
/*
class retVal {
public:
    int integer;
    string str;
    GrammerVar g_var;
    retVal() : integer(0),g_var(NOT_INITIALIZED){};
    retVal(char* tmp_yytext, GrammerVar var_type){
        g_var = var_type;
        switch (var_type){
            case IS_INT:
                integer = string_to_num(tmp_yytext);
                break;
            case IS_ID:
                str = string(tmp_yytext);
                break;
            case IS_STR:
                str = string(remove_double_quotes(tmp_yytext));
                break;
            case IS_ARRAY:
                //TODO
            case IS_CALL:
                //TODO
            default:
                throw;      //TODO
        }
    }*/
/*
    ~retVal(){
        if(g_var == IS_ID || g_var == IS_STR)
            free(str);
    }
    retVal(const retVal& tmp){
        *this = tmp;
    }
    retVal& operator=(const retVal& tmp){
        if(this != &tmp){
            str = nullptr;
            integer = tmp.integer;
            g_var = tmp.g_var;
            if(tmp.g_var == IS_STR || tmp.g_var == IS_ID)
                str = strdup(tmp.str);
        }
        return *this;
    }
*/
//};
//=========================== DATA TYPES ===============================//
class Type{
public:
    typeKind kind;

    int arrayLength;
    typeKind arrayType;

    Type() : kind(VOID), arrayLength(-1), arrayType(VOID) {};
    Type(typeKind Kind) : kind(Kind), arrayLength(-1){
        if(kind == ARRAY) assert(0); // this constructor should get only basic types
    }

    Type(typeKind Kind, int len) : kind(ARRAY), arrayLength(len), arrayType(Kind){
        if (Kind == ARRAY) assert(0);
    }

    bool operator==(const Type& toCompare){
        if(kind != ARRAY) return kind == toCompare.kind;
        return arrayLength == toCompare.arrayLength && arrayType == toCompare.arrayType;
    }

    int size(){
        return kind == ARRAY ? arrayLength:1;
    }
};

class VarBase{
public:
    string name;
    Type type;
    VarBase();
    VarBase(Type type_t) : name(), type(type_t){};
    VarBase(string str,typeKind kind): name(str), type(kind){};
    VarBase(string str, typeKind kind, int len):name(str), type(kind, len){};
};

class VarInfo: public VarBase{
public:
    int value;
    VarInfo() : value(0){};
    VarInfo(Type type): value(0),VarBase(type) {};
    varInfo(int val) : VarBase(string(),INTEGER) {};
    VarInfo(int val, string str, typeKind kind):
            value(val), VarBase(str,kind){};
    VarInfo(int val, string str, typeKind kind, int len):
            value(val), VarBase(str, kind, len){};
};

class Id : public VarBase{
public:
    int offset;

    Id(Type type, int offset, string name) : type(type), offset(offset), name(name){};

};

class parsedData{
public:
    enum DataKind {
        SINGLE,
        LIST,
        UNDEF
    };

    DataKind kind;
    VarInfo single_var;
    list<VarInfo> list_of_vars;

    parsedData() : kind(UNDEF){};
    parsedData(Type type):kind(SINGLE), single_var(type){ };
    parsedData(char* tmp_yytext, GrammerVar g_var) {
        kind = SINGLE;
        switch (g_var) {
            case IS_INT:
                single_var = VarInfo(string_to_num(tmp_yytext));
                break;
            case IS_ID:
                single_var = VarInfo(0,string(tmp_yytext), VOID);
                break;
            case IS_STR:
                single_var = VarInfo(0,string(remove_double_quotes(tmp_yytext)),STRING);
                break;
            case IS_ARRAY:
                //TODO
            case IS_CALL:
                //TODO
            default:
                throw;      //TODO
        }
    }
    parsedData(Type type, parsedData data) : kind(SINGLE){
        single_var.type = type;
        single_var.name = data.getName();
    }
    parsedData(parsedData data1, parsedData data2){
        if(data2.kind != UNDEF)
            kind = data2.kind;
        single_var = data1.single_var;
        if(data2.kind == SINGLE){   //we are in the NUM_T case
            single_var.type = type(BYTE);
        }
        else if(data2.kind == LIST){ //we are in the EXPLIST case
            list_of_vars = data2.list_of_vars;
            list_of_vars.push_front(data1.single_var);
        }
    }
    parsedData(parsedData data, GrammerVar g_var){
        if (g_var == IS_CALL){
            kind = LIST;
            list_of_vars = data.list_of_vars;
            if(data.list_of_vars.size() == 0)
                list_of_vars.push_front(data.single_var);
        }
    }

    Type getType(){
        return single_var.type;
    }
    string getName(){
        return single_var.name;
    }
    int getInteger(){
        return single_var.value;
    }
};

class parsedExp : public parsedData {
public:
    parsedExp();
    parsedExp(Type type) : parsedData(type){};
    parsedExp(parsedExp exp, binOps ops){
        if(ops == BOOL_OP)
            if (exp.isBool())
                return parsedExp(Type(BOOL));
        throw;      //TODO
    }
    parsedExp(parsedExp exp1, parsedExp exp2, binOps ops){
        switch (ops){
            case REL_OP:
                if(exp1.isInteger() && exp2.isInteger())
                    return parsedExp(Type(BOOL));
                else
                    throw;      //TODO
            case BOOL_OP:
                if(exp1.isBool() && exp2.isBool())
                    return parsedExp(Type(BOOL));
                else
                    throw;      //TODO
            case MATH_OP:
                try{
                    return maxRange(exp1, exp2);
                }
                catch (){
                    throw;      //TODO
                }
            default:
                throw;          //TODO
        }
    }
    bool isInteger(parsedExp exp){
        Type int_t = Type(INTEGER);
        Type byte_t = Type(BYTE);
        return (exp.getType()==int_t || exp.getType()==byte_t);
    }
    bool isBool(parsedExp exp){
        return exp.getType()==Type(BOOL);
    }
    Type maxRange(parsedExp exp1, parsedExp exp2) {
        Type int_t = Type(INTEGER);
        Type byte_t = Type(BYTE);
        if (exp1.getType() == int_t || exp2.getType() == int_t)
            return parsedExp(int_t);
        else if (exp1.getType() == byte_t && exp2.getType() == byte_t)
            return parsedExp(byte_t);
        else
            throw;      //TODO
    }
};

class function{
public:
    string idName;
    type return_type;
    list<Type> inputTypes;

    function(string idName, type return_type, list<Type> inputTypes) : string(string), return_type(return_type), inputTypes(inputTypes){};
};

#define YYSTYPE parsedData	// Tell Bison to use STYPE as the stack type
//=============================== GRAMMAR VARIABLES CLASSES ================//
/*
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
        g_var = IS_INT;
    }
    parsedNum(parsParmType type) : par_type(type), g_var(IS_INT){};
    parsedNum(retVal val, parsedNum type_num): retVal(val){
        g_var = IS_INT;
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
*/
 /*
class parsedStatement : public retVal{
public:
};
class parsedStatementList : public parsedStatement{
public:
    list<parsedStatement> state_list;
    parsedStatementList();
    parsedStatementList(parsedStatement state){
        state_list.push_front(state);
    }
    parsedStatementList(parsedStatement state, parsedStatementList state_list_t){
        state_list(state_list_t);
        state_list.push_front(state);
    }
};
*/
 /*
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
            default:
                throw;          //TODO Not supposed to get here!
        }
    }
};

class parsedCall : public parsedNum{
public:
    parsedExp call_id;
    parsedExpList exp_list;
    parsedCall() : g_var(IS_CALL) {
        call_id = parsedExp();
        exp_list = parsedExpList();
    }
    parsedCall(parsedExp id) : g_var(IS_CALL){
        call_id = id;
        exp_list = parsedExpList();
    }
    parsedCall(parsedExp id, parsedExpList exp_list_t) : g_var(IS_CALL){
        call_id = id;
        exp_list = exp_list_t;
    }

    parsedCall(const parsedCall& call){
        *this=call;
    }
    parsedCall& operator=(const parsedCall& call){
        call_id = call.call_id;
        exp_list = call.exp_list;
        return *this;
    }

};
*/
/*
class parsedExp : public parsedCall{
public:
    list<retVal> func_params;
    parsedExp(){};
    parsedExp(parsedNum num) : parsedNum(num){};
    parsedExp(bool b){
        par_type = BOOL;
        g_var = IS_BOOL;
        if(b)
            integer = 1;
        else
            integer = 0;
    }
    parsedExp(retVal val): parsedNum(val, NOTHING){
        if(g_var == IS_STR)
            par_type = STRING;
    }
    parsedExp(parsedExp exp1, parsedOp op, parsedExp exp2){
        *this= op.makeOp(exp1,exp2);
    }
    parsedExp(parsedCall call): parsedCall(call){};
    parsedExp(parsedExp id, parsedExp exp) : parsedExp(id){
        if (exp.g_var != IS_INT)
            throw;      //TODO
        g_var = IS_ARRAY;

    }

    parsedExp(const parsedExp& exp) : parsedCall(exp){};
    parsedExp& operator=(const parsedExp& exp){
        return parsedExp(exp);
    }


    //operators for the expression - I know it's a heck of a code duplication....
    parsedExp operator!(){
        if (par_type != BOOL)
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.g_var = IS_BOOL;
        new_exp.integer = 1-integer;
        return new_exp;
    };
    parsedExp operator&&(parsedExp exp2){
        if((par_type != BOOL )||(exp2.par_type != BOOL) )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.g_var = IS_BOOL;
        new_exp.integer = (integer==1 && exp2.integer ==1? 1 : 0);
        return new_exp;
    };
    parsedExp operator||(parsedExp exp2){
        return (!((!*this)&&(!exp2)));
    };
    parsedExp operator==(parsedExp exp2){
        if(g_var!=IS_INT || exp2.g_var!=IS_INT )
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type = BOOL;
        new_exp.g_var = IS_BOOL;
        new_exp.integer = (integer == exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator!=(parsedExp exp2){
        return (!(*this==exp2));
    };
    parsedExp operator<(parsedExp exp2){
        parsedExp new_exp = (*this==exp2);
        new_exp.integer = (integer < exp2.integer ? 1 : 0);
        return new_exp;
    };
    parsedExp operator<=(parsedExp exp2){
        return ((*this<exp2)||(*this==exp2));
    };
    parsedExp operator>(parsedExp exp2){
        return !(*this<=exp2);
    };
    parsedExp operator>=(parsedExp exp2){
        return !(*this<exp2);
    };
    parsedExp operator+(parsedExp exp2){
        if(g_var!=IS_INT || exp2.g_var!=IS_INT)
            throw;  //TODO
        parsedExp new_exp = parsedExp();
        new_exp.par_type =
                (par_type == INT ? INT : (exp2.par_type == INT ? INT : BYTE));
        new_exp.g_var = IS_INT;
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
};
*/
/*
class parsedExpList : public parsedExp{
public:
    list<parsedExp> exp_list;
    parsedExpList();
    parsedExpList(parsedExp exp){
        exp_list.push_front(exp);
    }
    parsedExpList(parsedExp exp, parsedExpList exp_list_t){
        exp_list(exp_list_t);
        exp_list.push_front(exp);
    }

    parsedExpList(const parsedExpList& expList1){
        *this = expList1;
    }
    parsedExpList& operator=(const parsedExpList& tmp){
        if(this != &tmp){
            str = nullptr;
            integer = tmp.integer;
            g_var = tmp.g_var;
            if(tmp.g_var == IS_STR || tmp.g_var == IS_ID)
                str = strdup(tmp.str);
            exp_list=tmp.exp_list;
        }
        return *this;
    }

};
*/
/*
class parsedIfElseSt : public parsedExp{
public:
    parsedStatement if_state;
    parsedStatement else_state;
    bool if_else;       //true  -> else_state is valid,
                        //false -> no else_state
    parsedIfElseSt(): if_else(false){};
    parsedIfElseSt(bool if_else_t ,parsedStatement ie_st) {
        if_else = false;
        if (if_else_t) {
            else_state(ie_st);
            if_else = true;
        } else
            if_state(ie_st);
    }
    parsedIfElseSt(parsedExp exp,parsedStatement st ,parsedIfElseSt ie_st)
            : parsedExp(exp), {
    }
};
*/

//=============================== SCOPE HANDLING ================//

class scope{
public:
    int nextIdLocation;
    list<Id> IdList;

    bool isWhileScope; // for the break command, make sure it is valid

    scope(){ // in case of the first scope scope
        IdList = new list<Id>;
    }

    scope(int nextIdLocation, bool isWhileScope) : nextIdLocation(nextIdLocation), isWhileScope(isWhileScope){
        IdList = new list<Id>;
    }

    ~scope(){
        printScope(); //to do :to build
        delete IdList;
    }

    void addId(type newIdType, string newIdName){

        IdList.insert (new Id(newIdType,nextIdLocation,newIdName));
        nextIdLocation += newIdType.size();
    }

    bool containsId(string name){
        for (Id id : IdList)
            if(id.name == name)
                return true;
        return false;
    }

    Id gedId(string name){
        for (Id id : IdList)
            if(id.name == name)
                return id;
        assert(0);
    }
};

class scopes{
public:
    list<function> functions;
    list<scope> scopesList;

    Id getId(string name){
        for(scope s : scopesList)
            for(Id id : s.IdList)
                if(id.name == name)
                    return id;
        assert(0);
    }

    function getFunction(string name){
        for(function fun : functions)
            if(fun.idName == name)
                return fun;
        assert(0);
    }

    bool containsIdName(string name){
        for(scope s : scopesList)
            for(Id id : s.IdList)
                if(id.name == name)
                    return true;
        return false;
    }

    bool containsFunctionName(string name){
        for(function f : functions)
            if(f.idName == name)
                return true;
        return false;
    }

    void addInitialFunction(Type returnType, string name, Type inputType){
        list<Type> functionInputTypes = new list<Type>;
        functionInputTypes.push_front(inputType);
        functions.push_front(new function(name, returnType, functionInputTypes));
    }

    void addFunction(parsedData retType,parsedData Id, parsedData functionInputs){
        assert(retType.kind == retType.SINGLE);
        assert(Id.kind == Id.SINGLE);
        assert(functionInputs.kind == functionInputs.LIST);

        //extracting the variables from the parsed data
        string name = Id.single_var.name;
        Type returnType = retType.single_var.type;

        list<Type> functionInputTypes = new list<Type>;
        while(!functionInputs.list_of_vars.empty()){
            functionInputTypes.push_back(functionInputs.list_of_vars.front().type);
            functionInputs.list_of_vars.pop_front();
        }

        // input check
        if(containsFunctionName(name)) throw {/* appropriate exception*/};
        if(name == "main" &&(returnType.kind != VOID || !functionInputTypes.empty())) throw {/* appropriate exception*/};

        // inserting to the function list
        functions.push_front(new function(name, returnType, functionInputTypes));
    }

    void addId(parsedData Id,parsedData type,parsedData isArray){
        assert(Id.kind == Id.SINGLE);
        assert(type.kind == type.SINGLE);

        if(containsIdName(Id.single_var.name)) throw {/* appropriate exception*/};

        Type newType;
        if(isArray.single_var.type.kind == isArray.single_var.type.ARRAY){
            newType.kind = newType.ARRAY;
            newType.arrayType = type.single_var.type.kind;
            newType.arrayLength = isArray.single_var.value;
        }
        else{
            newType.kind = type.single_var.type.kind;
        }

        scopesList.front().addId(new Id(newType,Id.single_var.name));
    }

    void addIdNotArray(parsedData type){
        scopesList.front().addId(new Id(type.single_var.type,type.single_var.name));
    }

    void newRegularScope(bool isWhileScope){

        int nextIdLocation = scopesList.front().nextIdLocation;
        bool oldIsInWhileScope = scopesList.front().isWhileScope;


        scopesList.push_front(new scope(nextIdLocation,oldIsInWhileScope | isWhileScope));
    }

    void newFunctionScope(parsedData inputVars){

        int newNextIdLocation = scopesList.front().nextIdLocation;
        scopesList.push_front(new scope(newNextIdLocation,false)); // false because opening a function means that we are not in a while scope

        // functions are always at the begin of the scope list, so for inserting the input values we will use a bit of
        // a hack, just manually insert the parameters to the next scope, without changing nextIdLocation.

        int i = -1;

        while(!inputVars.list_of_vars.empty()){
            scopesList.front().IdList.push_back(new Id(inputVars.list_of_vars.front().type.kind,i--,inputVars.list_of_vars.front().name));
            inputVars.list_of_vars.pop_front();
        }
    }

    void removeScope(){
        delete scopesList.pop_front();
    }

    void verifyAssign(parsedData Id,parsedData exp){
        if(!containsIdName(Id.single_var.name)) throw {/*  appropriate exception */}; // id not found

        Type idType = Id.single_var.type;
        if((!idType == exp.single_var.type) && // the types are different, and it is not a case of assign byte to int
           (!(idType == idType.INTEGER) && (exp.single_var.type == exp.single_var.type.BYTE)))
            throw {/*  appropriate exception */}; // incompatible types
    }

    void verifyAssignToArray(parsedData idInput, parsedData arrIndex, parsedData assigned){

        if(!containsIdName(id.single_var.name))
            throw {/*  appropriate exception */}; //id not found

        Type idType = getId(idInput.single_var.name)->type;
        Type indexType = arrIndex.single_var.type;
        Type assignedType = assigned.single_var.type;

        if(!idType.kind == idType.ARRAY)
            throw {/*  appropriate exception */}; // id is not array

        if((!indexType.kind == indexType.BYTE) &&
           (!indexType.kind == indexType.INTEGER))
            throw {/*  appropriate exception */}; // array index is not of numeric type

        if(!idType.arrayType == assignedType.kind && // the types are different, and it is not a case of assign byte to int
           (!(idType.arrayType == idType.INTEGER) && (assignedType.kind == assignedType.BYTE)))
            throw {/*  appropriate exception */}; // array type is not compatible with exp type
    }

    void verifyReturnTypeVoid(){
        Type temp(VOID);
        if(!functions.front().return_type == temp)
            throw {/*  appropriate exception */}; //function return different type
    }

    void verifyReturnType(parsedData returnType){
        if(!functions.front().return_type == returnType.single_var.type)
            throw {/*  appropriate exception */}; //function return different type
    }

    void verifyBreakBlock(){
        if(!scopesList.front().isWhileScope)
            throw {/*  appropriate exception */}; //break in middle of not while scope
    }

    void verifyExpIsBool(parsedData expType){
        type temp(BOOL);
        if(!expType.single_var.type == temp)
            throw {/*  appropriate exception */}; //expected boolean type
    }

    void verifyFunctionCall(parsedData idInput,parsedData inputList){
        assert(inputList.kind == inputList.LIST);

        if(!containsFunctionName(idInput.single_var.name))
            throw {/*  appropriate exception */}; //no such function

        list<Type> functionInputList = getFunction(idInput.single_var.name).inputTypes;
        list<VarInfo> actualInputTypes = inputList.list_of_vars;

        auto funIterator = functionInputList.cbegin();
        auto inputIterator = actualInputTypes.cbegin();

        while(funIterator != functionInputList.cend() && inputIterator != actualInputTypes.cend()){
            if(!funIterator == inputIterator->type)
                throw {/*  appropriate exception */}; //wrong function call parameters
            funIterator++;
            inputIterator++;
        }

        if((!funIterator == functionInputList.cend()) || (!inputIterator == actualInputTypes.cend()))
            throw {/*  appropriate exception */}; //wrong function call parameters number

    }

    void verifyNoParametersFunctionCall(parsedData idInput){
        if(!containsFunctionName(idInput.single_var.name))
            throw {/*  appropriate exception */}; //no such function

        if(!getFunction(idInput.single_var.name).inputTypes.empty())
            throw {/*  appropriate exception */}; //wrong function call parameters number
    }

    ~scopes(){
        while(!scopesList.empty())
            removeScope();

        while(!functions.empty()){
            functions.pop_front();
            //to do : build the printing function
        }
    };

};
#endif