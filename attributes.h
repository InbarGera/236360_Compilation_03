#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include <iostream>
#include <list>
#include <cassert>
#include <stdlib.h>
#include <sstream>
#include <exception>

#include "output.hpp"
#include "regAlloc.hpp"
#include "utills.hpp"


using std::cout;
using std::endl;
using std::list;
using std::string;
using namespace output;

#define PRINT_DEBUG 1

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
    parsingExceptions(errType err_type_t);
    parsingExceptions(errType err_type_t, string idName);
    parsingExceptions(errType err_type_t, int lineno_t);
    parsingExceptions(errType err_type_t, int lineno_t, string id_t);

    parsingExceptions(errType err_type_t, int lineno_t, string id_t, vector<string> args);
    static string intToString(int val);
    void printErrMsg();

    ~parsingExceptions() throw();

};


//=========================== DATA TYPES ====================================//
class Type{
public:
    enum typeKind {UNDEF, VOID, BOOL, INTEGER, BYTE, STRING, ARRAY};
    typeKind kind;

    int arrayLength;
    typeKind arrayType;

    Type();

    Type(typeKind Kind) ;

    Type(typeKind Kind, int len) ;

    bool operator==(const Type& toCompare);

    int size();

    static string typeKindToString(typeKind kind);

    string toString();
};

class VarBase{
public:
    string name;
    Type type;
    VarBase();
    VarBase(Type type_t) ;
    VarBase(string str,Type::typeKind kind);
    VarBase(string str, Type::typeKind kind, int len);
};

class VarInfo : public VarBase{
public:
    int value;
    VarInfo();
    VarInfo(Type type);
    VarInfo(int val);
    VarInfo(int val, string str, Type::typeKind kind);
    VarInfo(int val, string str, Type::typeKind kind, int len);
};

class Id : public VarBase{
public:
    int offset;

    Id(Type type_t, int offset, string name_t);
};

class function{
public:
    string idName;
    Type return_type;
    list<Type> inputTypes;
    function();
    function(string idName_t, Type return_type_t, list<Type> inputTypes_t);

    string toString();
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

    parsedData();

    parsedData(GrammerVar g_var);


    parsedData(Type type);
    parsedData(char* tmp_yytext, GrammerVar g_var);
    parsedData(Type type, parsedData& data);
    parsedData(Type type, string id);
    parsedData(Type type, int value);

    parsedData(parsedData& data1, parsedData& data2);
    parsedData(parsedData& data, GrammerVar g_var) ;
    parsedData(parsedData& data1, parsedData& data2, GrammerVar g_var);
    Type getType();
    string getName();
    int getInteger();
    bool isInteger();
    bool isBool();
    vector<string> getArgsTypes();

};

#define YYSTYPE parsedData*	// Tell Bison to use STYPE as the stack type

class parsedExp : public parsedData {
public:
    parsedExp();
    parsedExp(Type::typeKind kind);
    parsedExp(Type type) ;
    parsedExp(parsedExp exp, binOps ops);
    parsedExp(parsedExp exp1, parsedExp exp2, binOps ops);
    parsedExp(parsedData data);
    parsedExp(parsedData data1, parsedData data2, binOps ops);
    parsedExp(parsedData data, binOps ops);

    parsedExp maxRange(parsedExp exp1, parsedExp exp2);
};

//=============================== SCOPE HANDLING ============================//

class scope{
public:
    int nextIdLocation;
    list<Id> IdList;

    bool isWhileScope; // for the break command, make sure it is valid

    scope();// in case of the first scope scope

    scope(int nextIdLocation, bool isWhileScope);
    ~scope(){}

    void addId(Type newIdType, string newIdName);

    bool containsId(string name);

};

class scopes{
public:
    list<function> functions;
    list<scope> scopesList;
    bool need_to_print;
    scopes();
    Id getId(string name);
    function getFunction(string name);
    bool containsIdName(string name);
    bool containsFunctionName(string name);
    void addInitialFunction(Type returnType, string name, Type inputType);
    void addFunction(parsedData retType,parsedData Id, parsedData functionInputs);

    void addIdArray(parsedData Id,parsedData type,parsedData arraySize);

    void addIdNotArray(parsedData type);

    void newRegularScope(bool isWhileScope);
    void newFunctionScope(parsedData inputVars);
    void removeScope();

    void verifyAssign(parsedData Id,parsedData exp);

    void verifyAssignToArray(parsedData idInput, parsedData arrIndex, parsedData assigned);
    void verifyReturnTypeVoid();
    void verifyReturnType(parsedData returnType);
    void verifyBreakBlock();
    void verifyExpIsBool(parsedData expType);
    void verifyFunctionCall(parsedData idInput,parsedData inputList);
    void verifyNoParametersFunctionCall(parsedData idInput);

    static vector<string> listToVector(list<Type> list);

    ~scopes() ;
};

#endif