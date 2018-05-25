#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include "parser.tab.hpp"
#include <iostream>
#include <list>
#include <string>
#include <assert.h>
#include "output.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
using namespace output;
//================================= ENUMS ===============================//
enum GrammerVar{
    NOT_INITIALIZED,
    IS_BOOL,
    IS_INT,
    IS_ID,
    IS_STR,
    IS_CALL,
    IS_ARRAY
};

enum binOps {
    REL_OP,
    BOOL_OP,
    MATH_OP
};

//enum typeKind {VOID, BOOL, INTEGER, BYTE,  STRING,  ARRAY};

//=========================== HELPER FUNCTIONS =============================//
int string_to_num(char* input){
    int sum =0;
    while(input[0] != '\0'){
        sum = sum*10 + (input++[0] - '0');
    }
    return sum;
}

char* remove_double_quotes(char* input){
    char* res = malloc(strlen(input)* sizeof(char));
    strcpy(res,input+1);
    res[strlen(res) - 1] = '\0';
    return res;
}

//=========================== DATA TYPES ===============================//
class Type{
public:
    enum typeKind {VOID, BOOL, INTEGER, BYTE, STRING, ARRAY};
    typeKind kind;

    int arrayLength;
    typeKind arrayType;

    Type() : kind(VOID), arrayLength(-1) {
        arrayType = Type::typeKind::VOID;
    };

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

    static string typeKindToString(typeKind kind){
        switch(kind) {
            case VOID : {
                return "VOID";
            }
            case BOOL : {
                return "BOOL";
            }
            case INTEGER : {
                return "INTEGER";
            }
            case BYTE : {
                return "BYTE";
            }
            case STRING : {
                return "STRING";
            }
            case ARRAY : {
                return "ARRAY";
            }
            default: {
                assert(0);
            }
        }
    }

    string toString(){
        if(kind == ARRAY)
            return makeArrayType(typeKindToString(arrayType),arrayLength);
        return typeKindToString(kind);
    }
};

class VarBase{
public:
    string name;
    Type type;
    VarBase();
    VarBase(Type type_t) : name(), type(type_t){};
    VarBase(string str,Type::typeKind kind): name(str), type(kind){};
    VarBase(string str, Type::typeKind kind, int len):name(str), type(kind, len){};
};

class VarInfo: public VarBase{
public:
    int value;
    VarInfo() : value(0){};
    VarInfo(Type type): value(0),VarBase(type) {};
    VarInfo(int val) : VarBase(string(),Type::typeKind::INTEGER) {};
    VarInfo(int val, string str, Type::typeKind kind):
            value(val), VarBase(str,kind){};
    VarInfo(int val, string str, Type::typeKind kind, int len):
            value(val), VarBase(str, kind, len){};
};

class Id : public VarBase{
public:
    int offset;

    Id(Type type_t, int offset, string name_t) : offset(offset){
        this->type = type_t;
        this->name = name_t;
    };
};

class function{
public:
    string idName;
    Type return_type;
    list<Type> inputTypes;
    function();
    function(string idName_t, Type return_type_t, list<Type> inputTypes_t) :
            idName(idName_t), return_type(return_type_t), inputTypes(inputTypes_t){};

    string toString(){
        function temp = *this;
        list<string> inputTypes;
        while(!temp.inputTypes.empty()){
            Type temp2 = temp.inputTypes.back();
            inputTypes.push_front(temp2.toString());
            temp.inputTypes.pop_back();
        }

        return makeFunctionType(temp.return_type.toString(),inputTypes);
    }
};

//=============================== GRAMMAR VARIABLES CLASSES ================//

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
                single_var = VarInfo(0,string(tmp_yytext), Type::typeKind::VOID);
                break;
            case IS_STR:
                single_var = VarInfo(0,string(remove_double_quotes(tmp_yytext)),Type::typeKind::STRING);
                break;
            case IS_ARRAY:
                //TODO
            case IS_CALL:
                //TODO
            default:
                throw;      //TODO
        }
    }
    parsedData(Type type, parsedData& data) : kind(SINGLE){
        single_var.type = type;
        single_var.name = data.getName();
    }
    parsedData(parsedData& data1, parsedData& data2){
        if(data2.kind != UNDEF)
            kind = data2.kind;
        single_var = data1.single_var;
        if(data2.kind == SINGLE){   //we are in the NUM_T case
            single_var.type = Type(Type::typeKind::BYTE);
        }
        else if(data2.kind == LIST){ //we are in the EXPLIST case
            list_of_vars = data2.list_of_vars;
            list_of_vars.push_front(data1.single_var);
        }
    }
    parsedData(parsedData& data, GrammerVar g_var){
        if (g_var == IS_CALL){
            kind = LIST;
            list_of_vars = data.list_of_vars;
            if(data.list_of_vars.size() == 0)
                list_of_vars.push_front(data.single_var);
        }
    }
    bool isInteger(){
        Type int_t = Type(Type::typeKind::INTEGER);
        Type byte_t = Type(Type::typeKind::BYTE);
        return (getType()==int_t || getType()==byte_t);
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
#define YYSTYPE parsedData*	// Tell Bison to use STYPE as the stack type

class parsedExp : public parsedData {
public:
    parsedExp();
    parsedExp(Type type) : parsedData(type){};
    parsedExp(parsedExp exp, binOps ops){
        if(ops == BOOL_OP)
            if (exp.isBool()){
                *this =  parsedExp(Type(Type::typeKind::BOOL));
            }
        throw;      //TODO
    }
    parsedExp(parsedExp exp1, parsedExp exp2, binOps ops){
        switch (ops){
            case REL_OP:
                if(exp1.isInteger() && exp2.isInteger()) {
                    *this = parsedExp(Type(Type::typeKind::BOOL));
                }
                else
                    throw;      //TODO
            case BOOL_OP:
                if(exp1.isBool() && exp2.isBool()) {
                    *this = parsedExp(Type(Type::typeKind::BOOL));
                }
                else
                    throw;      //TODO
            case MATH_OP:
                try{
                    *this = maxRange(exp1, exp2);
                }
                catch (...){
                    throw;      //TODO
                }
            default:
                throw;          //TODO
        }
    }
    parsedExp(parsedData data) : parsedData(data){};
    parsedExp(parsedData data1, parsedData data2, binOps ops){
        *this = parsedExp(parsedExp(data1), parsedExp(data2), ops);
    }
    parsedExp(parsedData data, binOps ops){
        *this = parsedExp(parsedExp(data), ops);
    }


    bool isInteger(){
        Type int_t = Type(Type::typeKind::INTEGER);
        Type byte_t = Type(Type::typeKind::BYTE);
        return (getType()==int_t || getType()==byte_t);
    }
    bool isBool(){
        return getType()==Type(Type::typeKind::BOOL);
    }
    parsedData maxRange(parsedExp exp1, parsedExp exp2) {
        Type int_t = Type(Type::typeKind::INTEGER);
        Type byte_t = Type(Type::typeKind::BYTE);
        if (exp1.getType() == int_t || exp2.getType() == int_t)
            return parsedExp(int_t);
        else if (exp1.getType() == byte_t && exp2.getType() == byte_t)
            return parsedExp(byte_t);
        else
            throw;      //TODO
    }
};


//=============================== SCOPE HANDLING ================//

class scope{
public:
    int nextIdLocation;
    list<Id> IdList;

    bool isWhileScope; // for the break command, make sure it is valid

    scope(){} // in case of the first scope scope

    scope(int nextIdLocation, bool isWhileScope) : nextIdLocation(nextIdLocation), isWhileScope(isWhileScope){}

    ~scope(){}

    void addId(Type newIdType, string newIdName){

        Id temp(newIdType,nextIdLocation,newIdName);
        IdList.insert (temp);
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
        for(scope& s : scopesList)
            for(Id& id : s.IdList)
                if(id.name == name)
                    return id;
        assert(0);
    }

    function getFunction(string name){
        for(function& fun : functions)
            if(fun.idName == name)
                return fun;
        assert(0);
    }

    bool containsIdName(string name){
        for(scope& s : scopesList)
            for(Id& id : s.IdList)
                if(id.name == name)
                    return true;
        return false;
    }

    bool containsFunctionName(string name){
        for(function& f : functions)
            if(f.idName == name)
                return true;
        return false;
    }

    void addInitialFunction(Type returnType, string name, Type inputType){
        list<Type> functionInputTypes;
        functionInputTypes.push_front(inputType);

        function temp(name, returnType, functionInputTypes);
        functions.push_front(temp);
    }

    void addFunction(parsedData retType,parsedData Id, parsedData functionInputs){
        assert(retType.kind == retType.SINGLE);
        assert(Id.kind == Id.SINGLE);
        assert(functionInputs.kind == functionInputs.LIST);

        //extracting the variables from the parsed data
        string name = Id.single_var.name;
        Type returnType = retType.single_var.type;

        list<Type> functionInputTypes;
        while(!functionInputs.list_of_vars.empty()){
            functionInputTypes.push_back(functionInputs.list_of_vars.front().type);
            functionInputs.list_of_vars.pop_front();
        }

        // input check
        if(containsFunctionName(name)) errorUndefFunc(lineno,name);
        if(name == "main" &&(returnType.kind != returnType.VOID || !functionInputTypes.empty())) {}//throw {/* appropriate exception*/};

        // inserting to the function list
        function temp(name, returnType, functionInputTypes);
        functions.push_front(temp);
    }

    void addId(parsedData Id,parsedData type,parsedData isArray){
        assert(Id.kind == Id.SINGLE);
        assert(type.kind == type.SINGLE);

        if(containsIdName(Id.single_var.name)) {}//throw {/* appropriate exception*/};

        Type newType;
        if(isArray.single_var.type.kind == isArray.single_var.type.ARRAY){

            if(isArray.single_var.value < 0 || isArray.single_var.value > 256)
            {}//throw {/* appropriate exception*/}; // array size not good

            newType.kind = newType.ARRAY;
            newType.arrayType = type.single_var.type.kind;
            newType.arrayLength = isArray.single_var.value;
        }
        else{
            newType.kind = type.single_var.type.kind;
        }

        scopesList.front().addId(newType,Id.single_var.name);
    }

    void addIdNotArray(parsedData type){
        if(containsIdName(type.single_var.name)) {}//throw {/* appropriate exception*/};

        scopesList.front().addId(type.single_var.type,type.single_var.name);
    }

    void newRegularScope(bool isWhileScope){

        int nextIdLocation = scopesList.front().nextIdLocation;
        bool oldIsInWhileScope = scopesList.front().isWhileScope;

        scope temp(nextIdLocation,oldIsInWhileScope | isWhileScope);
        scopesList.push_front(temp);
    }

    void newFunctionScope(parsedData inputVars){

        int newNextIdLocation = scopesList.front().nextIdLocation;
        scope temp(newNextIdLocation,false);// false because opening a function means that we are not in a while scope
        scopesList.push_front(temp);

        // functions are always at the begin of the scope list, so for inserting the input values we will use a bit of
        // a hack, just manually insert the parameters to the next scope, without changing nextIdLocation.

        int i = -1;

        while(!inputVars.list_of_vars.empty()){
            Id temp(inputVars.list_of_vars.front().type,i--,inputVars.list_of_vars.front().name);
            scopesList.front().IdList.push_back(temp);
            inputVars.list_of_vars.pop_front();
        }
    }

    void removeScope(){
        scope oldScope = scopesList.front();

        // print the scope
        endScope();
        while(!oldScope.IdList.empty()){
            Id temp = oldScope.IdList.back();
            printID(temp.name,temp.offset,temp.type.toString());
            oldScope.IdList.pop_back();
        }

        scopesList.pop_front();
    }

    void verifyAssign(parsedData Id,parsedData exp){
        if(!containsIdName(Id.single_var.name)) {}//throw {/*  appropriate exception */}; // id not found

        Type idType = Id.single_var.type;
        if((!(idType == exp.single_var.type)) && // the types are different, and it is not a case of assign byte to int
           (!(idType.kind == idType.INTEGER) && (exp.single_var.type.kind == exp.single_var.type.BYTE)))
        {}//throw {/*  appropriate exception */}; // incompatible types
    }

    void verifyAssignToArray(parsedData idInput, parsedData arrIndex, parsedData assigned){

        if(!containsIdName(idInput.single_var.name))
        {}//throw {/*  appropriate exception */}; //id not found

        Type idType = getId(idInput.single_var.name).type;
        Type indexType = arrIndex.single_var.type;
        Type assignedType = assigned.single_var.type;

        if(!(idType.kind == idType.ARRAY))
        {}//throw {/*  appropriate exception */}; // id is not array

        if((!(indexType.kind == indexType.BYTE)) &&
           (!(indexType.kind == indexType.INTEGER)))
        {}//throw {/*  appropriate exception */}; // array index is not of numeric type

        if(!(idType.arrayType == assignedType.kind )&& // the types are different, and it is not a case of assign byte to int
           (!(idType.arrayType == idType.INTEGER) && (assignedType.kind == assignedType.BYTE)))
        {}//throw {/*  appropriate exception */}; // array type is not compatible with exp type
    }

    void verifyReturnTypeVoid(){
        Type temp(Type::typeKind::VOID);
        if(!(functions.front().return_type == temp))
        {}//throw {/*  appropriate exception */}; //function return different type
    }

    void verifyReturnType(parsedData returnType){
        if((!(functions.front().return_type == returnType.single_var.type)) &&
           !((functions.front().return_type == functions.front().return_type.INTEGER) &&
             (returnType.single_var.type == returnType.single_var.type.BYTE)))
        {}//throw {/*  appropriate exception */}; //function return different type
    }

    void verifyBreakBlock(){
        if(!scopesList.front().isWhileScope)
        {}//throw {/*  appropriate exception */}; //break in middle of not while scope
    }

    void verifyExpIsBool(parsedData expType){
        Type temp(Type::typeKind::BOOL);
        if(!(expType.single_var.type == temp))
        {}//throw {/*  appropriate exception */}; //expected boolean type
    }

    void verifyFunctionCall(parsedData idInput,parsedData inputList){
        assert(inputList.kind == inputList.LIST);

        if(!containsFunctionName(idInput.single_var.name))
        {}//throw {/*  appropriate exception */}; //no such function

        list<Type> functionInputList = getFunction(idInput.single_var.name).inputTypes;
        list<VarInfo> actualInputTypes = inputList.list_of_vars;

        auto funIterator = functionInputList.cbegin();
        auto inputIterator = actualInputTypes.cbegin();

        while(funIterator != functionInputList.cend() && (inputIterator != actualInputTypes.cend())){
            Type tmp = *funIterator;
            if(!(tmp == inputIterator->type))
            {}//throw {/*  appropriate exception */}; //wrong function call parameters
            funIterator++;
            inputIterator++;
        }

        if((!(funIterator == functionInputList.cend())) || (!(inputIterator == actualInputTypes.cend())))
        {}//throw {/*  appropriate exception */}; //wrong function call parameters number

    }

    void verifyNoParametersFunctionCall(parsedData idInput){
        if(!containsFunctionName(idInput.single_var.name))
        {}//throw {/*  appropriate exception */}; //no such function

        if(!getFunction(idInput.single_var.name).inputTypes.empty())
        {}//throw {/*  appropriate exception */}; //wrong function call parameters number
    }

    ~scopes() {
        while (!scopesList.empty())
            removeScope();

        while (!functions.empty()) {
            function func = functions.front();
            printID(func.idName, 0, func.toString());
        }
    }
};

#endif