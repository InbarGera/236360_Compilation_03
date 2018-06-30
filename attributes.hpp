#ifndef __ATTRIBUTES_H
#define __ATTRIBUTES_H

#include <cstring>
#include <iostream>
#include <list>
#include <cassert>
#include <stdlib.h>
#include <sstream>
#include <exception>
#include <string>
#include <vector>

#include "output.hpp"
#include "regAlloc.hpp"
#include "utills.hpp"
#include "bp.hpp"

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
    IS_ARRAY,
    IS_OP
};

enum binOps {
    REL_OP,
    BOOL_OP,
    MATH_OP
};

//====================== ERROR HANDLING CLASS ===============================//
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
/*
class HW5Exceptions : public parsingExceptions{
public:
    enum semanticsErr{
        ERR_INDEX_OUT,
        ERR_DIV_BY_ZERO
    };
    semanticsErr semantic_err;
    HW5Exceptions(semanticsErr err_t) : semantic_err(err_t){
        print_now = false;
    };
    ~HW5Exceptions(){
        string label_to_jump;
        switch (semantic_err){
            case ERR_INDEX_OUT:
                label_to_jump = string("arrayErrHandle");
            case ERR_DIV_BY_ZERO:
                label_to_jump = string("divZeroErrHandle");
        }
        buffer.emit(string("j ")+label_to_jump);
    }
};*/
//=========================== DATA TYPES ====================================//
class BPInfo {
public:
    enum registerType{
        REG_TYPE_VALUE,
        REG_TYPE_REFERENCE,
        REG_TYPE_UNDEF
    };
    enum BoolType{
        BOOL_TYPE_VALUE,
        BOOL_TYPE_BRANCH_LISTS,
        BOOL_TYPE_UNDEF
    };

    regClass reg; //same name as in VarInfo, make sure to change both if one is changed.
    registerType regType; //same name as in VarInfo, make sure to change both if one is changed.
    BoolType boolType;

    string beginLabel;
    std::vector<int> trueList;
    std::vector<int> falseList;
    std::vector<int> nextList;
    std::vector<int> breakList;

    string ifTrueLabel; //relevant only for if statements
    string ifFalseLabel; //relevant only for if statements

    BPInfo(); // NOT generate new label
    BPInfo(string label); // copying input label
};

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

class VarInfo : public VarBase, public BPInfo{
public:
    //enum registerType{value,reference,undef}; // /*HW5*/ same as in parsedExp, make sure to change both if one is changed.
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
    Id(); // for HW5
};

class function{
public:
    string idName;
    string beginLabel; //new for HW5, necessary for calling the function (jump to this label)
    Type return_type;
    list<Type> inputTypes;
    function();
    function(string idName_t, Type return_type_t, list<Type> inputTypes_t);

    string toString();
};

//========================= GRAMMAR VARIABLES CLASSES =======================//
class parsedData{
public:
    enum DataKind {
        DK_OP,
        DK_SINGLE,
        DK_ARRAY,
        DK_LIST,
        DK_UNDEF
    };
    enum PDOp{
        PD_NO_OP,
        PD_PLUS,
        PD_MINUS,
        PD_MUL,
        PD_DIV,
        PD_AND,
        PD_OR,
        PD_NOT,
        PD_EQ,     // ==
        PD_NEQ,    // !=
        PD_GT,     // >
        PD_GEQ,    // >=
        PD_LT,     // <
        PD_LEQ     // <=
    };
    DataKind kind;
    VarInfo single_var;
    PDOp pd_op;
    list<VarInfo> list_of_vars;

    parsedData();

    parsedData(GrammerVar g_var);

    parsedData(Type type);
    parsedData(char* tmp_yytext, GrammerVar g_var);
    parsedData(Type type, parsedData& data);
    parsedData(Type type, string id);
    parsedData(Type type, int value);
    parsedData(PDOp pd_op_t) : kind(DK_OP), pd_op(pd_op_t){};

    parsedData(parsedData& data1, parsedData& data2);
    parsedData(parsedData& data, GrammerVar g_var) ;
    parsedData(parsedData& data1, parsedData& data2, GrammerVar g_var);
    Type getType();
    string getName();
    int getInteger();
    bool isInteger();
    bool isBool();
    vector<string> getArgsTypes();
    PDOp stringToOp(string parsed_op_t);

    bool isAritOp( ){
        assert(kind == DK_OP);
        return  (pd_op >= PD_PLUS && pd_op <= PD_DIV);
    }
    bool isBoolOp(){
        assert(kind == DK_OP);
        return  (pd_op >= PD_AND && pd_op <= PD_NOT);
    }
    bool isRelOp(){
        assert(kind == DK_OP);
        return  (pd_op >= PD_EQ && pd_op <= PD_LEQ);
    }

};
#define YYSTYPE parsedData*	// Tell Bison to use STYPE as the stack type

class parsedExp : public parsedData, public BPInfo {
public:
    //enum registerType{value,reference,undef}; // same as in VarInfo, make sure to change both if one is changed.

    parsedExp(Type::typeKind kind);
    parsedExp(Type type) ;
    parsedExp(parsedExp exp, binOps ops);
    parsedExp(parsedExp exp1, parsedExp exp2, binOps ops);
    parsedExp(parsedData data);
    parsedExp(parsedData data1, parsedData data2, binOps ops);
    parsedExp(parsedData data, binOps ops);

    parsedExp(parsedExp e1,parsedExp e2);
    parsedExp maxRange(parsedExp exp1, parsedExp exp2);

    bool isBoolExp();
};

class parsedStatement : public parsedData, public BPInfo {
public:
    parsedStatement(); // NOT creates new label (originally did create)
    parsedStatement(parsedExp exp); // copying exp's label
    parsedStatement(string label);

};

//=============================== SCOPE HANDLING ============================//
class scope{
public:
    int nextIdLocation;
    list<Id> IdList;

    bool insideWhileScope; // for the break command, make sure it is valid
    bool isWhileScope;
    scope();// in case of the first scope scope

    scope(int nextIdLocation, bool insideWhileScope_t, bool isWhileScope_t);
    ~scope(){}

    void addId(Type newIdType, string newIdName);

    bool containsId(string name);

};

class scopes{
public:
    list<function> functions;
    list<scope> scopesList;
    bool need_to_print;
    //CodeBuffer buffer;  //addition for hw5
    scopes();
    Id getId(string name);
    int getIdOffset(string name);
    function getFunction(string name);
    bool containsIdName(string name);
    bool containsFunctionName(string name);
    void addInitialFunction(Type returnType, string name, Type inputType);
    void addFunction(parsedData retType,parsedData Id, parsedData functionInputs, parsedExp labelExp);

    void addIdArray(parsedData Id,parsedData type,parsedData arraySize);
    void addIdNotArray(parsedData type);

    void newRegularScope(bool insideWhileScope_t, bool isWhileScope_t);
    void newFunctionScope(parsedData inputVars);
    void removeScope();
    void removeScopeVars(scope to_remove);

    vector<scope> findFirstWhileScope();

    void verifyAssign(parsedData Id,parsedExp exp);
    void verifyAssignToArray(parsedData idInput, parsedExp arrIndex, parsedExp assigned);
    void verifyReturnTypeVoid();
    void verifyReturnType(parsedData returnType);
    void verifyBreakBlock();
    void verifyExpIsBool(parsedData expType);
    void verifyFunctionCall(parsedData idInput,parsedData inputList);
    void verifyNoParametersFunctionCall(parsedData idInput);

    static vector<string> listToVector(list<Type> list);

    ~scopes();
};

class codeGenerator{
public:
    static void initiateHW5();
    static void initiateKnownConstants();
    static void initiateErrorHandling();
    static void initiatePrintFunctions();
    static string opToBranchString(parsedData::PDOp op);
    static string arithmeticOpToString(parsedData::PDOp op);
    static void byteArithmeticMasking(regClass reg);
    static string divisionByZeroCheck(regClass reg);
    static void generateArrayOverflowCheck(int arrayLen,regClass reg);
    static string idOffsetFromFp(Id id);
    static void assignValueToId(Id id,parsedExp reg);
    static void assignValueToArray(Id id,parsedExp offsetExp,parsedExp reg);
    static string trueValueRepresentation();
    static string falseValueRepresentation();
    static void assignBoolIntoLocation(parsedExp exp,regClass destination);
    static void assignNonBoolIntoLocation(parsedExp exp,regClass destination);
    static void assignBoolIntoV1(parsedExp exp);

    static string pushStringToDataBuffer(parsedData toPush);
    static void assignArrayToArray(Id id,parsedExp exp);
    static void generateArrayLocationCalc(regClass destenetion, regClass offsetHoldingReg, string offsetOfBaseFromFp);
    static int getIdOffset(string name); // dangerous, scopes have same function
    static void pushExpListAndFreeRegisters(parsedExp input_list);
    static void callerSaveRegisters(vector<regClass> regsToSave);
    static void callerRestoreRegisters(vector<regClass> regsToSave);
    static string IntToReg(int reg_to_save);
    static void cleanStack();
    static void returnVoidFunction();
    static void returnFunction(parsedExp returnExp);
    static void callFunction(string func_name, parsedExp input_list,regClass returnReg);

    static void putIdAddressInRegister(Id id, regClass reg);
    static void generateNewSingleVarCreation(parsedExp exp);


};

#endif
