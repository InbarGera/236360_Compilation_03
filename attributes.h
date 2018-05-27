#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include <iostream>
#include <list>
#include <cassert>
#include <stdlib.h>
#include "output.hpp"
#include <sstream>a
#include <exception>


using std::cout;
using std::endl;
using std::list;
using std::string;
using namespace output;

#define PRINT_DEBUG 0

//================================= ENUMS ==================================//
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
//=========================== HELPER FUNCTIONS =============================//
static int string_to_num(char* input){
    int sum =0;
    while(input[0] != '\0'){
        sum = sum*10 + (input++[0] - '0');
    }
    return sum;
}

static char* remove_double_quotes(char* input){
    char* res = (char*)malloc(strlen(input)* sizeof(char));
    strcpy(res,input+1);
    res[strlen(res) - 1] = '\0';
    return res;
}

//============================ ERROR HANDLING ===============================//
class parsingExceptions : std::exception {
public:
    enum errType {
        ERR_LEX,
        ERR_SYN,
        ERR_UNDEF,
        ERR_DEF,
        ERR_UNDEF_FUN,
        ERR_MISMATCH,
        ERR_PROTOTYPE_MISMATCH,
        ERR_UNEXPECTED_BREAK,
        ERR_MAIN_MISSING,
        ERR_BYTE_TOO_LARGE,
        ERR_INVALID_ARRAY_SIZE,
        ERR_UNKNOWN_ERROR
    };
    bool print_now;
    errType err_type;
    int lineno;
    string id;
    vector<string> argTypes;
    parsingExceptions(errType err_type_t) :print_now(false), err_type(err_type_t){};
    parsingExceptions(errType err_type_t, int lineno_t) : print_now(true),
                                                          err_type(err_type_t), lineno(lineno_t){};
    parsingExceptions(errType err_type_t, int lineno_t, string id_t):print_now(true),
                                                                     err_type(err_type_t), lineno(lineno_t), id(id_t){};
    parsingExceptions(errType err_type_t, int lineno_t, string id_t, vector<string> argTypes_t) :
            print_now(true), err_type(err_type_t), lineno(lineno_t), argTypes(argTypes_t){};
    void printErrMsg(){
        switch (err_type) {
            case ERR_LEX: {
                errorLex(lineno);
            }
                break;
            case ERR_SYN: {
                errorSyn(lineno);
            }
                break;
            case ERR_UNDEF: {
                errorUndef(lineno, id);
            }
                break;
            case ERR_DEF: {
                errorDef(lineno, id);
            }
                break;
            case ERR_UNDEF_FUN: {
                errorUndefFunc(lineno, id);
            }
                break;
            case ERR_MISMATCH: {
                errorMismatch(lineno);
            }
                break;
            case ERR_PROTOTYPE_MISMATCH: {
                errorPrototypeMismatch(lineno, id, argTypes);
            }
                break;
            case ERR_UNEXPECTED_BREAK: {
                errorUnexpectedBreak(lineno);
            }
                break;
            case ERR_MAIN_MISSING: {
                errorMainMissing();
            }
                break;
            case ERR_BYTE_TOO_LARGE: {
                errorByteTooLarge(lineno, id);
            }
                break;
            case ERR_INVALID_ARRAY_SIZE: {
                errorInvalidArraySize(lineno, id);
            }
                break;
            case ERR_UNKNOWN_ERROR: {
                if (PRINT_DEBUG) cout << "received an unknown error!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            }
                break;
        }
    }
    ~parsingExceptions() throw() {
        if (print_now)
            printErrMsg();
    }

};


//=========================== DATA TYPES ====================================//
class Type{
public:
    enum typeKind {VOID, BOOL, INTEGER, BYTE, STRING, ARRAY};
    typeKind kind;

    int arrayLength;
    typeKind arrayType;

    Type() : kind(VOID), arrayLength(-1) {
        arrayType = Type::VOID;
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
                return string("VOID");
            }
            case BOOL : {
                return string("BOOL");
            }
            case INTEGER : {
                return string("INT");
            }
            case BYTE : {
                return string("BYTE");
            }
            case STRING : {
                return string("STRING");
            }
            case ARRAY : {
                return string("ARRAY");
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
    VarBase(){};
    VarBase(Type type_t) : name(), type(type_t){};
    VarBase(string str,Type::typeKind kind): name(str), type(kind){};
    VarBase(string str, Type::typeKind kind, int len):name(str), type(kind, len){};
};

class VarInfo : public VarBase{
public:
    int value;
    VarInfo() : value(0){};
    VarInfo(Type type): value(0),VarBase(type) {};
    VarInfo(int val) : value(val), VarBase(string(),Type::INTEGER) {};
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
        vector<string> inputTypes;
        while(!temp.inputTypes.empty()){
            Type temp2 = temp.inputTypes.back();
            inputTypes.push_back(temp2.toString());
            temp.inputTypes.pop_back();
        }

        return makeFunctionType(temp.return_type.toString(),inputTypes); // to reverse inputTypes
    }
};

//=============================== GRAMMAR VARIABLES CLASSES =================//

class parsedData{
public:
    enum DataKind {
        SINGLE,
        ARRAY,
        LIST,
        UNDEF
    };

    DataKind kind;
    VarInfo single_var;
    list<VarInfo> list_of_vars;

    parsedData() : kind(UNDEF){ };

    parsedData(GrammerVar g_var) : kind(LIST){
        if(g_var != IS_CALL)
            assert(0);
    }


    parsedData(Type type) : kind(SINGLE), single_var(type){};
    parsedData(char* tmp_yytext, GrammerVar g_var) {
        kind = SINGLE;
        switch (g_var) {
            case IS_INT: {
                single_var = VarInfo(string_to_num(tmp_yytext));
            }
                break;
            case IS_ID: {
                single_var = VarInfo(0, string(tmp_yytext), Type::VOID);
            }
                break;
            case IS_STR: {
                char *tmp = remove_double_quotes(tmp_yytext);
                single_var = VarInfo(0, string(tmp), Type::STRING);
                free(tmp);
            }
                break;
            case IS_ARRAY: {
                //TODO
            }
                break;
            case IS_CALL: {
                //TODO
            }
                break;
            default: {
                if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(char*,GrammarVar)" << endl;
                throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
            }
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
        single_var.name = data2.single_var.name;
        switch (data2.kind){
            case SINGLE: {
                single_var.type = data2.single_var.type;
                single_var.value = data1.single_var.value;
                if(single_var.type.kind == Type::BYTE) {
                    if (data1.single_var.value > 255 || data1.single_var.value < 0)
                        throw parsingExceptions(parsingExceptions::ERR_BYTE_TOO_LARGE);
                }
            }break;
            case ARRAY: {
                single_var.type =
                        Type(data1.single_var.type.kind, data2.single_var.type.arrayLength);
            }break;
            case LIST: {
                list_of_vars = data2.list_of_vars;
                list_of_vars.push_front(data1.single_var);
            }break;
            default: {
                if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData, parsedData)" << endl;
                throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
            }
        }

    }
    parsedData(parsedData& data, GrammerVar g_var) {
        if (g_var == IS_CALL) {
            kind = LIST;
            list_of_vars = data.list_of_vars;
            if (data.list_of_vars.size() == 0)
                list_of_vars.push_front(data.single_var);
        } else if (g_var == IS_ARRAY) {
            kind = ARRAY;
            single_var.type =
                    Type(data.single_var.type.kind, data.single_var.value);
        } else {
            if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData,GrammerVar)" << endl;
            throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
        }
    }
    parsedData(parsedData& data1, parsedData& data2, GrammerVar g_var){
        if(g_var == IS_ARRAY){
            kind = ARRAY;
            Type int_t = Type(Type::INTEGER);
            Type byte_t = Type(Type::BYTE);
            if((data2.getType() == int_t || data2.getType() == byte_t)
               && (data2.getInteger()>0 && data2.getInteger() <256 )){
                single_var.name = data1.single_var.name;
                single_var.type =
                        Type(data1.single_var.type.kind, data2.single_var.value );
            }
            else
                throw parsingExceptions(parsingExceptions::ERR_INVALID_ARRAY_SIZE);      //TODO
        }
        else {
            if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData,parsedData,GrammerVar)" << endl;
            throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
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
    bool isInteger(){
        Type int_t = Type(Type::INTEGER);
        Type byte_t = Type(Type::BYTE);
        return (getType()==int_t || getType()==byte_t);
    }
    bool isBool(){
        return getType()==Type(Type::BOOL);
    }
    vector<string> getArgsTypes(){
        assert(kind == LIST);
        vector<string> tmp;
        while(!list_of_vars.empty()){
            tmp.push_back(list_of_vars.front().name);
            list_of_vars.pop_front();
        }
        return tmp;
    }

};

#define YYSTYPE parsedData*	// Tell Bison to use STYPE as the stack type

class parsedExp : public parsedData {
public:
    parsedExp();
    parsedExp(Type type) : parsedData(type){};
    parsedExp(parsedExp exp, binOps ops) {
        if (ops == BOOL_OP) {
            if (exp.isBool()) {
                *this = parsedExp(Type(Type::BOOL));
            } else
                throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        } else {
            if (PRINT_DEBUG) cout << "reached a default case in constructor parsedExp(parsedExp,binOps)" << endl;
            throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
        }
    }
    parsedExp(parsedExp exp1, parsedExp exp2, binOps ops){
        switch (ops){
            case REL_OP: {
                if (exp1.isInteger() && exp2.isInteger()) {
                    *this = parsedExp(Type(Type::BOOL));
                } else
                    throw parsingExceptions(parsingExceptions::ERR_MISMATCH);      //TODO
            }break;
            case BOOL_OP: {
                if (exp1.isBool() && exp2.isBool()) {
                    *this = parsedExp(Type(Type::BOOL));
                } else
                    throw parsingExceptions(parsingExceptions::ERR_MISMATCH);      //TODO
            }break;
            case MATH_OP: {
                if(!exp1.isInteger() || !exp2.isInteger())
                    throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
                *this = maxRange(exp1, exp2);   //maxRange already throws the compatible error
            }break;
            default: {
                if (PRINT_DEBUG) cout << "reached a default case in constructor parsedExp(parsedExp,parsedExp,binOps)" << endl;
                throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
            }        }
    }
    parsedExp(parsedData data) : parsedData(data){};
    parsedExp(parsedData data1, parsedData data2, binOps ops){
        *this = parsedExp(parsedExp(data1), parsedExp(data2), ops);
    }
    parsedExp(parsedData data, binOps ops){
        *this = parsedExp(parsedExp(data), ops);
    }



    parsedExp maxRange(parsedExp exp1, parsedExp exp2) {
        Type int_t = Type(Type::INTEGER);
        Type byte_t = Type(Type::BYTE);
        if (exp1.getType() == int_t || exp2.getType() == int_t)
            return parsedExp(int_t);
        else if (exp1.getType() == byte_t && exp2.getType() == byte_t)
            return parsedExp(byte_t);
        else
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);      //TODO
    }
};

//=============================== SCOPE HANDLING ============================//

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
        IdList.push_front(temp);
        nextIdLocation += newIdType.size();
    }
};

class scopes{
public:
    list<function> functions;
    list<scope> scopesList;
    bool need_to_print;
    scopes() : need_to_print(true){};
    Id getId(string name){
        list<scope> allScopes(scopesList);
        if (PRINT_DEBUG) cout << "trying to GET the  ID:  " << name << endl;
        if(allScopes.size() == 0)
            assert(0);

        for(list<scope>::iterator s = allScopes.begin(); s != allScopes.end() ; s++) {
            if((*s).IdList.size() == 0)
                continue;
            for (list<Id>::iterator id = (*s).IdList.begin(); id !=(*s).IdList.end() ; id++){
                if (PRINT_DEBUG) cout << "checking if " << (*id).name << " == " << name << endl;
                if ((*id).name == name)
                    return *id;
            }
        }
        assert(0);
    }
    function getFunction(string name){
        list<function> funcs = functions;

        if(funcs.size() == 0)
            assert(0);

        for(list<function>::iterator fun = funcs.begin(); fun != funcs.end(); fun ++)
            if((*fun).idName == name)
                return *fun;

        assert(0);
    }
    bool containsIdName(string name){
        list<scope> allScopes(scopesList);

        if(allScopes.size() == 0)
            return false;
        if (PRINT_DEBUG) cout << "trying to find (bool) ID:  " << name << endl;
        for(list<scope>::iterator s = allScopes.begin(); s != allScopes.end() ; s++) {
            if((*s).IdList.size() == 0)
                continue;
            for (list<Id>::iterator id = (*s).IdList.begin(); id !=(*s).IdList.end() ; id++){
                if (PRINT_DEBUG) cout << "checking if " << (*id).name << " == " << name << endl;
                if ((*id).name == name)
                    return true;
            }
        }
        return false;
    }
    bool containsFunctionName(string name){
        list<function> funcs = functions;

        if(funcs.size() == 0)
            return false;
        for(list<function>::iterator fun = funcs.begin(); fun != funcs.end(); fun ++)
            if((*fun).idName == name)
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
        //assert(functionInputs.kind == functionInputs.LIST);

        //extracting the variables from the parsed data
        string name = Id.single_var.name;
        Type returnType = retType.single_var.type;

        list<Type> functionInputTypes;
        while(!functionInputs.list_of_vars.empty()){
            functionInputTypes.push_back(functionInputs.list_of_vars.front().type);
            functionInputs.list_of_vars.pop_front();
        }

        // input check
        if(containsFunctionName(name)) {
            throw parsingExceptions(parsingExceptions::ERR_DEF);
        }//errorUndefFunc(lineno,name); assert
        if(name == "main" &&(returnType.kind != returnType.VOID || !functionInputTypes.empty())) {
            throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
        }//throw {/* appropriate exception*/};

        // inserting to the function list
        function temp(name, returnType, functionInputTypes);
        functions.push_front(temp);
    }
    void addIdArray(parsedData Id,parsedData type,parsedData arraySize){
        assert(Id.kind == Id.SINGLE);
        assert(type.kind == type.SINGLE);

        if(containsIdName(Id.single_var.name)) {
            throw parsingExceptions(parsingExceptions::ERR_DEF);
        }//throw {/* appropriate exception*/};

        Type newType;

        if(arraySize.single_var.value < 0 || arraySize.single_var.value > 256)
        {
            throw parsingExceptions(parsingExceptions::ERR_INVALID_ARRAY_SIZE);
        }//throw {/* appropriate exception*/}; // array size not good

        newType.kind = newType.ARRAY;
        newType.arrayType = type.single_var.type.kind;
        newType.arrayLength = arraySize.single_var.value;

        scopesList.front().addId(newType,Id.single_var.name);

        if (PRINT_DEBUG) cout << "\n\n\nadded array: " << Id.single_var.name << endl;
    }
    void addIdNotArray(parsedData type){
        if(containsIdName(type.single_var.name)) {
            throw parsingExceptions(parsingExceptions::ERR_DEF);
        }//throw {/* appropriate exception*/};

        scopesList.front().addId(type.single_var.type,type.single_var.name);
        if (PRINT_DEBUG) cout << "\n\n\nadded ID (not array): " << type.single_var.name << endl;

    }
    void newRegularScope(bool isWhileScope){

        int nextIdLocation = scopesList.front().nextIdLocation;
        bool oldIsInWhileScope = scopesList.front().isWhileScope;

        scope temp(nextIdLocation,oldIsInWhileScope | isWhileScope);
        scopesList.push_front(temp);
    }
    void newFunctionScope(parsedData inputVars){

        int newNextIdLocation = scopesList.front().nextIdLocation;
        scope newScope(newNextIdLocation,false);// false because opening a function means that we are not in a while scope

        // functions are always at the begin of the scope list, so for inserting the input values we will use a bit of
        // a hack, just manually insert the parameters to the next scope, without changing nextIdLocation.

        int i = -1;

        while(!inputVars.list_of_vars.empty()){
            Id temp(inputVars.list_of_vars.front().type,i,inputVars.list_of_vars.front().name);
            newScope.IdList.push_back(temp);
            inputVars.list_of_vars.pop_front();
            i -= inputVars.list_of_vars.front().type.size();
        }
        scopesList.push_front(newScope);
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
        if(!containsIdName(Id.single_var.name)) {
            throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN);
        }//throw {/*  appropriate exception */}; // id not found

        Type idType = Id.single_var.type;
        if((!(idType == exp.single_var.type)) && // the types are different, and it is not a case of assign byte to int
           (!(idType.kind == idType.INTEGER) && (exp.single_var.type.kind == exp.single_var.type.BYTE)))
        {
            throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
        }//throw {/*  appropriate exception */}; // incompatible types
    }
    void verifyAssignToArray(parsedData idInput, parsedData arrIndex, parsedData assigned){

        if(!containsIdName(idInput.single_var.name))
        {
            throw parsingExceptions(parsingExceptions::ERR_UNDEF);
        }//throw {/*  appropriate exception */}; //id not found

        Type idType = getId(idInput.single_var.name).type;
        Type indexType = arrIndex.single_var.type;
        Type assignedType = assigned.single_var.type;

        if(!(idType.kind == idType.ARRAY))
        {
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        }//throw {/*  appropriate exception */}; // id is not array

        if((!(indexType.kind == indexType.BYTE)) &&
           (!(indexType.kind == indexType.INTEGER)))
        {
            throw parsingExceptions(parsingExceptions::ERR_INVALID_ARRAY_SIZE);
        }//throw {/*  appropriate exception */}; // array index is not of numeric type

        if(!(idType.arrayType == assignedType.kind )&& // the types are different, and it is not a case of assign byte to int
           (!(idType.arrayType == idType.INTEGER) && (assignedType.kind == assignedType.BYTE)))
        {
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        }//throw {/*  appropriate exception */}; // array type is not compatible with exp type
    }
    void verifyReturnTypeVoid(){
        Type temp(Type::VOID);
        if(!(functions.front().return_type == temp))
        {
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        }//throw {/*  appropriate exception */}; //function return different type
    }
    void verifyReturnType(parsedData returnType){
        if((!(functions.front().return_type == returnType.single_var.type)) &&
           !((functions.front().return_type == functions.front().return_type.INTEGER) &&
             (returnType.single_var.type == returnType.single_var.type.BYTE)))
        {
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        }//throw {/*  appropriate exception */}; //function return different type
    }
    void verifyBreakBlock(){
        if(!scopesList.front().isWhileScope)
        {
            throw parsingExceptions(parsingExceptions::ERR_UNEXPECTED_BREAK);
        }//throw {/*  appropriate exception */}; //break in middle of not while scope
    }
    void verifyExpIsBool(parsedData expType){
        Type temp(Type::BOOL);
        if(!(expType.single_var.type == temp))
        {
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
        }//throw {/*  appropriate exception */}; //expected boolean type
    }
    void verifyFunctionCall(parsedData idInput,parsedData inputList){
        assert(inputList.kind == parsedData::LIST);

        if(!containsFunctionName(idInput.single_var.name))
        {
            throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN);
        }//throw {/*  appropriate exception */}; //no such function

        list<Type> functionInputList = getFunction(idInput.single_var.name).inputTypes;
        list<VarInfo> actualInputTypes = inputList.list_of_vars;

        list<Type>::iterator funIterator = functionInputList.begin();
        list<VarInfo>::iterator inputIterator = actualInputTypes.begin();

        while(funIterator != functionInputList.end() && (inputIterator != actualInputTypes.end())){
            Type tmp = *funIterator;
            if(!(tmp == inputIterator->type))
            {
                throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
            }//throw {/*  appropriate exception */}; //wrong function call parameters
            funIterator++;
            inputIterator++;
        }

        if((!(funIterator == functionInputList.end())) || (!(inputIterator == actualInputTypes.end())))
        {
            throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
        }//throw {/*  appropriate exception */}; //wrong function call parameters number

    }
    void verifyNoParametersFunctionCall(parsedData idInput){
        if(!containsFunctionName(idInput.single_var.name))
        {
            throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN);
        }//throw {/*  appropriate exception */}; //no such function

        if(!getFunction(idInput.single_var.name).inputTypes.empty())
        {
            throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
        }//throw {/*  appropriate exception */}; //wrong function call parameters number
    }
    ~scopes() {
        if(need_to_print) {
            while (!scopesList.empty())
                removeScope();

            while (!functions.empty()) {
                function func = functions.back();
                printID(func.idName, 0, func.toString());
                functions.pop_back();
            }
        }
    }
};

#endif