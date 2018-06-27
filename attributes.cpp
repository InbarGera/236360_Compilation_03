#include "attributes.hpp"

CodeBuffer buffer;
scopes* scopesList;
vector<int> goToMainBackPatch;
//===========================================================================//
//====================== ERROR HANDLING CLASS ===============================//
//===========================================================================//
parsingExceptions::parsingExceptions(errType err_type_t)
        :print_now(false), err_type(err_type_t){};
parsingExceptions::parsingExceptions(errType err_type_t,string idName)
        :print_now(false), id(idName), err_type(err_type_t){};
parsingExceptions::parsingExceptions(errType err_type_t, int lineno_t)
        : print_now(true), err_type(err_type_t), lineno(lineno_t){};
parsingExceptions::parsingExceptions(errType err_type_t, int lineno_t, string id_t)
        :print_now(true), err_type(err_type_t), lineno(lineno_t), id(id_t){};
parsingExceptions::parsingExceptions(errType err_type_t, int lineno_t, string id_t, vector<string> args) :
            print_now(true), err_type(err_type_t), id(id_t), lineno(lineno_t), argTypes(args){};
string parsingExceptions::intToString(int val){
        int i=0;
        int mod =1;
        char retVal[100];

        if(val == 0){
            retVal[0] = '0';
            retVal [1] = '\0';
            if(PRINT_DEBUG) cout << " in num translation, original num is : " << val << "translation is: " << retVal << endl;
            return string(retVal);
        }

        while(val/mod)
            mod*=10;
        mod /= 10;
        while(mod){
            retVal[i++] = (char)((val/mod % 10) +'0');
            mod /= 10;
        }
        retVal[i]='\0';
        if(PRINT_DEBUG) cout << " in num translation, original num is : " << val << "translation is: " << retVal << endl;
        return string(retVal);
    }
void parsingExceptions::printErrMsg(){
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
                //if(PRINT_DEBUG) cout << "in ERR_PROTOTYPE_MISMATCH, argTypes.front() = " << argTypes.front() << endl;
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
                if(PRINT_DEBUG) cout << "got into the case, id = " << id << endl;
                errorByteTooLarge(lineno,id);
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
parsingExceptions::~parsingExceptions() throw() {
        if (print_now)
            printErrMsg();
    }
//===========================================================================//
//=========================== DATA TYPES ====================================//
//===========================================================================//

//=========================== TYPE CLASS ====================================//
Type::Type() : kind(UNDEF), arrayLength(-1) {
        arrayType = Type::UNDEF;
    };
Type::Type(typeKind Kind) : kind(Kind), arrayLength(-1){
        if(kind == ARRAY) assert(0); // this constructor should get only basic types
    }
Type::Type(typeKind Kind, int len) : kind(ARRAY), arrayLength(len), arrayType(Kind){
        if (Kind == ARRAY) assert(0);
    }
bool Type::operator==(const Type& toCompare){
        if(kind != ARRAY) return kind == toCompare.kind;
        return arrayLength == toCompare.arrayLength && arrayType == toCompare.arrayType;
    }
int Type::size(){
        return kind == ARRAY ? arrayLength:1;
    }
string Type::typeKindToString(typeKind kind){
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
string Type::toString(){
        if(kind == ARRAY)
            return makeArrayType(typeKindToString(arrayType),arrayLength);
        return typeKindToString(kind);
    }

//=========================== VarBase CLASS =================================//
VarBase::VarBase(){};
VarBase::VarBase(Type type_t) : name(), type(type_t){};
VarBase::VarBase(string str,Type::typeKind kind): name(str), type(kind){};
VarBase::VarBase(string str, Type::typeKind kind, int len):name(str), type(kind, len){};

//=========================== VarInfo CLASS =================================//
VarInfo::VarInfo() : value(0), BPInfo(){};
VarInfo::VarInfo(Type type): value(0),VarBase(type), BPInfo() {};
VarInfo::VarInfo(int val) : value(val), VarBase(string(),Type::INTEGER), BPInfo() {};
VarInfo::VarInfo(int val, string str, Type::typeKind kind):
            value(val), VarBase(str,kind), BPInfo(){};
VarInfo::VarInfo(int val, string str, Type::typeKind kind, int len):
            value(val), VarBase(str, kind, len), BPInfo(){};

//============================= ID CLASS ====================================//
Id::Id(Type type_t, int offset, string name_t) : offset(offset){
        this->type = type_t;
        this->name = name_t;
    };

Id::Id(){
    offset = -9876543; // or something better ?
    this->type = Type(Type::UNDEF);
    this->name = "______undefined_id______";
}
//=========================== FUNCTION CLASS ================================//
function::function(string idName_t, Type return_type_t, list<Type> inputTypes_t) :
            idName(idName_t), return_type(return_type_t), inputTypes(inputTypes_t){};
string function::toString(){
        function temp = *this;
        vector<string> inputTypes;
        while(!temp.inputTypes.empty()){
            Type temp2 = temp.inputTypes.front();
            inputTypes.push_back(temp2.toString());
            temp.inputTypes.pop_front();
        }

        return makeFunctionType(temp.return_type.toString(),inputTypes); // to reverse inputTypes
    }

//===========================================================================//
//========================== GRAMMAR VARIABLES CLASSES ======================//
//===========================================================================//

//=========================== ParsedData Class ==============================//
parsedData::parsedData() : kind(DK_UNDEF){ };
parsedData::parsedData(GrammerVar g_var) : kind(DK_LIST), pd_op(PD_NO_OP){
        if(g_var != IS_CALL)
            assert(0);
    }
parsedData::parsedData(Type type) : single_var(type), pd_op(PD_NO_OP){
        kind = type.kind == Type::ARRAY ? DK_ARRAY : DK_SINGLE;
    };
parsedData::parsedData(char* tmp_yytext, GrammerVar g_var) : pd_op(PD_NO_OP){
        kind = DK_SINGLE;
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
                //TODO - not supposed to get here
                if (PRINT_DEBUG) cout << "something wierd happend!!!"
                                      << " constructor yytext, g_var , IS_ARRAY"
                                      << endl;
            }
                break;
            case IS_CALL: {
                //TODO - not supposed to get here
                if (PRINT_DEBUG) cout << "something wierd happend!!!"
                                      << " constructor yytext, g_var , IS_CALL"
                                      << endl;
            }
                break;
            case IS_OP:{
                kind = DK_OP;
                pd_op = stringToOp(string(tmp_yytext));
            }
                break;
            default: {
                if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(char*,GrammarVar)" << endl;
                throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
            }
        }
    }
parsedData::parsedData(Type type, parsedData& data) : kind(DK_SINGLE),pd_op(PD_NO_OP){
        single_var.type = type;
        single_var.name = data.getName();
    }
parsedData::parsedData(Type type, string id): pd_op(PD_NO_OP){ // for TYPE_ID -> TYPE ID
        single_var = VarInfo(type);
        single_var.name = id;
    }
parsedData::parsedData(Type type, int value): pd_op(PD_NO_OP){

        if(PRINT_DEBUG) cout << "inside parsedData(Type type, int value, value = " << value << endl;


        if(type.kind == Type::BYTE && (value<0||value>255))
            throw parsingExceptions(parsingExceptions::ERR_BYTE_TOO_LARGE,parsingExceptions::intToString(value));
        kind = DK_SINGLE;
        single_var.value = value;
        single_var.type = type;
    }
parsedData::parsedData(parsedData& data1, parsedData& data2): pd_op(PD_NO_OP){
        if(data2.kind != DK_UNDEF)
            kind = data2.kind;

        single_var = data1.single_var;
        single_var.name = data2.single_var.name;
        switch (data2.kind){
            case DK_SINGLE: {
                single_var.type = data2.single_var.type;
                single_var.value = data1.single_var.value;
                if(single_var.type.kind == Type::BYTE) {
                    if (single_var.value > 255 || single_var.value < 0)
                        throw parsingExceptions(parsingExceptions::ERR_BYTE_TOO_LARGE,parsingExceptions::intToString(single_var.value));
                }
            }break;
            case DK_ARRAY: {
                single_var.type =
                        Type(data1.single_var.type.kind, data2.single_var.type.arrayLength);
            }break;
            case DK_LIST: {
                list_of_vars = data2.list_of_vars;
                list_of_vars.push_front(data1.single_var);
            }break;
            default: {
                if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData, parsedData)" << endl;
                throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
            }
        }

    }
parsedData::parsedData(parsedData& data, GrammerVar g_var): pd_op(PD_NO_OP) {
        if (g_var == IS_CALL) {
            kind = DK_LIST;
            list_of_vars = data.list_of_vars;
            if (data.list_of_vars.size() == 0)
                list_of_vars.push_front(data.single_var);
        } else if (g_var == IS_ARRAY) {
            kind = DK_ARRAY;
            single_var.type =
                    Type(data.single_var.type.kind, data.single_var.value);
        } else {
            if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData,GrammerVar)" << endl;
            throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
        }
    }
parsedData::parsedData(parsedData& data1, parsedData& data2, GrammerVar g_var): pd_op(PD_NO_OP){
        if(g_var == IS_ARRAY){
            kind = DK_ARRAY;
            Type int_t = Type(Type::INTEGER);
            Type byte_t = Type(Type::BYTE);

            if(PRINT_DEBUG) {
                cout << "in parsedData(parsedData& data1, parsedData& data2, GrammerVar g_var), data2.single_var.value =  " << data2.single_var.value << endl;
            }

            if((data2.getType() == int_t || data2.getType() == byte_t)
               && (data2.getInteger()>0 && data2.getInteger() <256 )){
                single_var.name = data1.single_var.name;
                single_var.type = Type(data1.single_var.type.kind, data2.single_var.value );
            }
            else
                throw parsingExceptions(parsingExceptions::ERR_INVALID_ARRAY_SIZE, data1.single_var.name);      //TODO
        }
        else {
            if (PRINT_DEBUG) cout << "reached a default case in constructor parsedData(parsedData,parsedData,GrammerVar)" << endl;
            throw parsingExceptions(parsingExceptions::ERR_UNKNOWN_ERROR);      //TODO
        }
    }
Type parsedData::getType(){
        return single_var.type;
    }
string parsedData::getName(){
        return single_var.name;
    }
int parsedData::getInteger(){
        return single_var.value;
    }
bool parsedData::isInteger(){
        Type int_t = Type(Type::INTEGER);
        Type byte_t = Type(Type::BYTE);
        return (getType()==int_t || getType()==byte_t);
    }
bool parsedData::isBool(){
        return getType().kind == Type::BOOL;
    }
vector<string> parsedData::getArgsTypes(){
        assert(kind == DK_LIST);
        vector<string> tmp;
        while(!list_of_vars.empty()){
            tmp.push_back(list_of_vars.front().name);
            list_of_vars.pop_front();
        }
        return tmp;
    }

parsedData::PDOp parsedData::stringToOp(string parsed_op_t){
    if (parsed_op_t == "+")
        return PD_PLUS;
    if (parsed_op_t == "-")
        return PD_MINUS;
    if(parsed_op_t == "*")
        return PD_MUL;
    if(parsed_op_t == "/")
        return PD_DIV;
    if(parsed_op_t == "and")
        return PD_AND;
    if(parsed_op_t == "or")
        return PD_OR;
    if(parsed_op_t == "not")
        return  PD_NOT;
    if(parsed_op_t == "==")
        return PD_EQ;
    if(parsed_op_t == "!=")
        return PD_NEQ;
    if(parsed_op_t == ">")
        return PD_GT;
    if(parsed_op_t == ">=")
        return PD_GEQ;
    if(parsed_op_t == "<")
        return PD_LT;
    if(parsed_op_t == "<=")
        return PD_LEQ;
    assert(0);   // NOT SUPPOSED TO GET HERE!!!!!!!
}

//=========================== BPInfo Class ===============================//

BPInfo::BPInfo() : beginLabel("undef"){}

BPInfo::BPInfo(string label) : beginLabel(label){}


//=========================== ParsedExp Class ===============================//

parsedExp::parsedExp(Type::typeKind kind): parsedData(Type(kind)), regType(REG_TYPE_UNDEF){}
parsedExp::parsedExp(Type type) : parsedData(type), regType(REG_TYPE_UNDEF){};
parsedExp::parsedExp(parsedExp exp, binOps ops): regType(REG_TYPE_UNDEF) {
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
parsedExp::parsedExp(parsedExp exp1, parsedExp exp2, binOps ops): regType(REG_TYPE_UNDEF){
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
parsedExp::parsedExp(parsedData data) : parsedData(data), regType(REG_TYPE_UNDEF){};
parsedExp::parsedExp(parsedData data1, parsedData data2, binOps ops): regType(REG_TYPE_UNDEF){
        *this = parsedExp(parsedExp(data1), parsedExp(data2), ops);
    }
parsedExp::parsedExp(parsedData data, binOps ops): regType(REG_TYPE_UNDEF){
        *this = parsedExp(parsedExp(data), ops);
    }
parsedExp parsedExp::maxRange(parsedExp exp1, parsedExp exp2) {
        Type int_t = Type(Type::INTEGER);
        Type byte_t = Type(Type::BYTE);
        if (exp1.getType() == int_t || exp2.getType() == int_t)
            return parsedExp(int_t);
        else if (exp1.getType() == byte_t && exp2.getType() == byte_t)
            return parsedExp(byte_t);
        else
            throw parsingExceptions(parsingExceptions::ERR_MISMATCH);      //TODO
    }

parsedExp::parsedExp(parsedExp e1,parsedExp e2): parsedData(e1,e2){}

bool parsedExp::isBoolExp(){
    return regType == REG_TYPE_UNDEF; // (!(trueList.empty() && falseList.empty())) old code
}
//=========================== ParsedStatement Class ===============================//

parsedStatement::parsedStatement(){
    BPInfo();
}
parsedStatement::parsedStatement(parsedExp exp){
    trueList = exp.trueList;
    falseList = exp.falseList;
    nextList = exp.nextList;
    breakList = exp.breakList;
}

parsedStatement::parsedStatement(string label){
    BPInfo();
    beginLabel = label;
}

//===========================================================================//
//=============================== SCOPE HANDLING ============================//
//===========================================================================//

//=============================== SCOPE CLASS ===============================//
scope::scope(){} // in case of the first scope scope
scope::scope(int nextIdLocation, bool isWhileScope) : nextIdLocation(nextIdLocation), isWhileScope(isWhileScope){}
void scope::addId(Type newIdType, string newIdName){

        Id temp(newIdType,nextIdLocation,newIdName);
        IdList.push_front(temp);
        nextIdLocation += newIdType.size();
    }
bool scope::containsId(string name){
        list<Id> IdNames = IdList;
        while(!IdNames.empty()){
            if(IdNames.front().name == name)
                return true;
            IdNames.pop_front();
        }
        return false;
    }

//============================== SCOPES CLASS ===============================//
scopes::scopes() : need_to_print(true){};
Id scopes::getId(string name){
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

int scopes::getIdOffset(string name){
    Id temp = getId(name);
    return temp.offset;
}

function scopes::getFunction(string name){
        list<function> funcs = functions;

        if(funcs.size() == 0)
            assert(0);

        for(list<function>::iterator fun = funcs.begin(); fun != funcs.end(); fun ++)
            if((*fun).idName == name)
                return *fun;

        assert(0);
    }
bool scopes::containsIdName(string name){
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
bool scopes::containsFunctionName(string name){
        list<function> funcs = functions;

        if(funcs.size() == 0)
            return false;
        for(list<function>::iterator fun = funcs.begin(); fun != funcs.end(); fun ++)
            if((*fun).idName == name)
                return true;
        return false;
    }
void scopes::addInitialFunction(Type returnType, string name, Type inputType){
        list<Type> functionInputTypes;
        functionInputTypes.push_front(inputType);

        function temp(name, returnType, functionInputTypes);
        functions.push_front(temp);
    }
void scopes::addFunction(parsedData retType,parsedData Id, parsedData functionInputs){
        assert(retType.kind == retType.DK_SINGLE);
        assert(Id.kind == Id.DK_SINGLE);

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
            throw parsingExceptions(parsingExceptions::ERR_DEF,name);
        }//errorUndefFunc(lineno,name); assert
        /*   if(name == "main" &&(returnType.kind != returnType.VOID || !functionInputTypes.empty())) {
               throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH,Id.getName());
           } */ //throw {/* appropriate exception*/};

        // inserting to the function list
        function temp(name, returnType, functionInputTypes);
        functions.push_front(temp);
    }
void scopes::addIdArray(parsedData Id,parsedData type,parsedData arraySize) {
    if (containsIdName(Id.single_var.name) || containsFunctionName(Id.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_DEF, Id.single_var.name);
    }//throw {/* appropriate exception*/};
    Type newType;
    if (arraySize.single_var.value < 1 || arraySize.single_var.value > 255) {
        throw parsingExceptions(parsingExceptions::ERR_INVALID_ARRAY_SIZE,
                                Id.single_var.name);
    }//throw {/* appropriate exception*/}; // array size not good
    newType.kind = Type::ARRAY;
    newType.arrayType = type.single_var.type.kind;
    newType.arrayLength = arraySize.single_var.value;
    scopesList.front().addId(newType, Id.single_var.name);

    if (PRINT_DEBUG) cout << "\n\n\nadded array: " << Id.single_var.name << endl;
}
void scopes::addIdNotArray(parsedData type) {
    if (containsIdName(type.single_var.name) || containsFunctionName(type.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_DEF, type.single_var.name);
    }//throw {/* appropriate exception*/};

    scopesList.front().addId(type.single_var.type, type.single_var.name);
    if (PRINT_DEBUG) cout << "\n\n\nadded ID (not array): " << type.single_var.name << endl;
}
void scopes::newRegularScope(bool isWhileScope) {

    int nextIdLocation = scopesList.front().nextIdLocation;
    bool oldIsInWhileScope = scopesList.front().isWhileScope;

    scope temp(nextIdLocation, oldIsInWhileScope | isWhileScope);
    scopesList.push_front(temp);
}
void scopes::newFunctionScope(parsedData inputVars) {

    int newNextIdLocation = scopesList.front().nextIdLocation;
    scope newScope(newNextIdLocation, false);// false because opening a function means that we are not in a while scope

    // functions are always at the begin of the scope list, so for inserting the input values we will use a bit of
    // a hack, just manually insert the parameters to the next scope, without changing nextIdLocation.

    int i = 0;

    while (!inputVars.list_of_vars.empty()) {
        string name = inputVars.list_of_vars.front().name;
        Type type = inputVars.list_of_vars.front().type;
        i -= inputVars.list_of_vars.front().type.size();

        if (containsIdName(name) || newScope.containsId(name) || containsFunctionName(name))
            throw parsingExceptions(parsingExceptions::ERR_DEF, name);

        Id temp(type, i, name);
        newScope.IdList.push_front(temp);
        inputVars.list_of_vars.pop_front();
    }
    scopesList.push_front(newScope);
}
void scopes::removeScope() {
    scope oldScope = scopesList.front();

    // print the scope
    endScope();
    while (!oldScope.IdList.empty()) {
        Id temp = oldScope.IdList.back();
        printID(temp.name, temp.offset, temp.type.toString());
        oldScope.IdList.pop_back();
    }

    scopesList.pop_front();
}
void scopes::verifyAssign(parsedData id_t,parsedExp exp) {
    if (!containsIdName(id_t.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN, id_t.single_var.name); // id not found
    }

    Type idType = getId(id_t.single_var.name).type;
    if (!(idType == exp.single_var.type) && // the types are different, and it is not a case of assign byte to int
        !((idType.kind == Type::INTEGER) && (exp.single_var.type.kind == Type::BYTE))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH); // incompatible types
    }
}
void scopes::verifyAssignToArray(parsedData idInput, parsedExp arrIndex, parsedExp assigned) {

    if (!containsIdName(idInput.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF, idInput.single_var.name);
    }//id not found

    Id id = getId(idInput.single_var.name);
    Type idType = id.type;
    Type indexType = arrIndex.single_var.type;
    Type assignedType = assigned.single_var.type;

    if (!(idType.kind == Type::ARRAY)) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }// id is not array

    if ((!(indexType.kind == Type::BYTE)) &&
        (!(indexType.kind == Type::INTEGER))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }// array index is not of numeric type

    if (!(idType.arrayType == assignedType.kind) &&
        // the types are different, and it is not a case of assign byte to int
        !((idType.arrayType == idType.INTEGER) && (assignedType.kind == assignedType.BYTE))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }// array type is not compatible with exp type

}
void scopes::verifyReturnTypeVoid() {
    if (!(functions.front().return_type.kind == Type::VOID)) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; //function return different type
}
void scopes::verifyReturnType(parsedData returnType) {
    if (PRINT_DEBUG) cout << "in verifyReturnType " << endl;

    if (functions.front().return_type.kind == Type::VOID)
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);

    if ((!(functions.front().return_type == returnType.single_var.type)) &&
        !((functions.front().return_type == Type::INTEGER) &&
          (returnType.single_var.type == Type::BYTE))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; //function return different type
}
void scopes::verifyBreakBlock() {
    if (!scopesList.front().isWhileScope) {
        throw parsingExceptions(parsingExceptions::ERR_UNEXPECTED_BREAK);
    }//throw {/*  appropriate exception */}; //break in middle of not while scope
}
void scopes::verifyExpIsBool(parsedData expType) {
    Type temp(Type::BOOL);
    if (!(expType.single_var.type == temp)) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; //expected boolean type
}
void scopes::verifyFunctionCall(parsedData idInput,parsedData inputList) {
    assert(inputList.kind == parsedData::DK_LIST);

    if (!containsFunctionName(idInput.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN, idInput.single_var.name);
    }//throw {/*  appropriate exception */}; //no such function

    list <Type> functionInputList = getFunction(idInput.single_var.name).inputTypes;
    list <VarInfo> actualInputTypes = inputList.list_of_vars;

    list<Type>::iterator funIterator = functionInputList.begin();
    list<VarInfo>::iterator inputIterator = actualInputTypes.begin();

    while (funIterator != functionInputList.end() && (inputIterator != actualInputTypes.end())) {


        if (PRINT_DEBUG) {
            cout << "function expected type : ";
            printID("", 0, funIterator->toString());
            cout << "actual input type : ";
            printID("", 0, inputIterator->type.toString());
        }

        Type funExpectedType = *funIterator;
        Type actualInputType = inputIterator->type;

        if (!(funExpectedType == actualInputType) &&
            !(funExpectedType.kind == Type::INTEGER && actualInputType.kind == Type::BYTE)) {

            throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH, idInput.getName());
        }//throw {/*  appropriate exception */}; //wrong function call parameters
        funIterator++;
        inputIterator++;
    }

    if ((!(funIterator == functionInputList.end())) || (!(inputIterator == actualInputTypes.end()))) {
        throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH, idInput.getName());
    }

}
void scopes::verifyNoParametersFunctionCall(parsedData idInput) {
    if (!containsFunctionName(idInput.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN, idInput.single_var.name);
    }//throw {/*  appropriate exception */}; //no such function

    if (!getFunction(idInput.single_var.name).inputTypes.empty()) {
        throw parsingExceptions(parsingExceptions::ERR_PROTOTYPE_MISMATCH);
    }//throw {/*  appropriate exception */}; //wrong function call parameters number
}
vector<string> scopes::listToVector(list<Type> list) {
    vector <string> res;
    while (!list.empty()) {
        res.push_back(list.front().toString());
        list.pop_front();
    }

    if (PRINT_DEBUG) cout << " in listToVector, res front is : " << res.front() << endl;
    return res;
}
scopes::~scopes() {
    if (need_to_print) {
        while (!scopesList.empty())
            removeScope();

        while (!functions.empty()) {
            function func = functions.back();
            printID(func.idName, 0, func.toString());
            functions.pop_back();
        }
    }

}

//============================== codeGenerator CLASS ===============================//

void codeGenerator::initiateHW5(){
    buffer.emit(string("# Begin of initiation code"));
    buffer.emit(string("main:"));
    buffer.emit(string("la $ra, return_from_main"));
    buffer.emit(string("subu $fp, $sp, 4"));
    goToMainBackPatch.push_back(buffer.emit(string("j "))); //will be backpached when main will be parsed
    buffer.emit(string("return_from_main:"));
    buffer.emit(string("nop"));
    initiateKnownConstants();
    initiateErrorHandling();
    initiatePrintFunctions();
    buffer.emit(string("# End of initiation code"));
}

void codeGenerator::initiateKnownConstants(){
    static bool initiated = false;
    if(initiated)
        return;

    buffer.emitData(string("byteMask: .word 0x000000ff"));
    buffer.emitData(string("arrayIndexErrMsg: .asciiz  \"Error index out of bounds\\n\""));
    buffer.emitData(string("divByZeroErrMsg: .asciiz  \"Error division by zero\\n\""));

    initiated = true;
}

void codeGenerator::initiateErrorHandling(){
    buffer.emit(string("arrayErrHandle: li $v0, 4"));
    buffer.emit(string("la $a0, arrayIndexErrMsg"));
    buffer.emit(string("syscall"));
    buffer.emit(string("nop"));

    buffer.emit(string("divZeroErrHandle: li $v0, 4"));
    buffer.emit(string("la $a0, divByZeroErrMsg"));
    buffer.emit(string("syscall"));
    buffer.emit(string("nop"));
}

void codeGenerator::initiatePrintFunctions(){

    buffer.emit(string("printiFunc: lw $a0, 0($sp)"));
    buffer.emit(string("li $v0, 1"));
    buffer.emit(string("syscall"));
    buffer.emit(string("jr $ra"));

    buffer.emit(string("printFunc: lw $a0, 0($sp)"));
    buffer.emit(string("li $v0, 4"));
    buffer.emit(string("syscall"));
    buffer.emit(string("jr $ra"));
}

string codeGenerator::opToBranchString(parsedData::PDOp op){
    switch(op){
        case parsedData::PD_EQ :{  // ==
            return "beq ";
        }break;
        case parsedData::PD_NEQ :{  // !=
            return "bne ";
        }break;
        case parsedData::PD_GT :{  // >
            return "bgt ";
        }break;
        case parsedData::PD_GEQ :{  // >=
            return "bge ";
        }break;
        case parsedData::PD_LT :{  // <
            return "blt ";
        }break;
        case parsedData::PD_LEQ :{  // <=
            return "ble ";
        }break;
        default:{
            assert(0);
        }
    }
    assert(0);
}

string codeGenerator::arithmeticOpToString(parsedData::PDOp op){
    switch(op) {
        case parsedData::PD_PLUS : {
            return "add ";
        }break;
        case parsedData::PD_MINUS : {
            return "sub ";
        }break;
        case parsedData::PD_MUL : {
            return "mul ";
        }break;
        case parsedData::PD_DIV : {
            return "div ";
        }break;
        default:{
            assert(0);
        }
    }
    assert(0);
}

string codeGenerator::byteArithmeticMasking(regClass reg){
    string res = "and ";
    res += reg.toString();
    res += ", ";
    res += reg.toString();
    res += ", ";
    res += "byteMask";
    return res;
}

string codeGenerator::divisionByZeroCheck(regClass reg){
    string res = "beq ";
    res += reg.toString();
    res += ", $0, ";
    res += "divZeroErrHandle"; //code for reporting division by 0 and exit;
    return res;
}

string codeGenerator::idOffsetFromFp(Id id){
    return num_to_string(-(id.offset)*4);
}

void codeGenerator::assignBoolIntoLocation(parsedExp exp, regClass destination){

    string res;
    buffer.bpatch(exp.trueList,buffer.genLabel());

    regClass tempReg = regAlloc();
    buffer.emit(string("add ") + tempReg.toString() + string(", $0, ") + trueValueRepresentation());
    buffer.emit(string("sw ") + tempReg.toString() + string(", (") + destination.toString() + string(")"));

    vector<int> temp;
    temp.push_back(buffer.emit(string("j ")));
    buffer.bpatch(exp.falseList,buffer.genLabel());

    buffer.emit(string("add ") + tempReg.toString() + string(", $0, ") + falseValueRepresentation());
    buffer.emit(string("sw ") + tempReg.toString() + string(", (") + destination.toString() + string(")"));
    regFree(tempReg);

    buffer.bpatch(temp,buffer.genLabel());
}

void codeGenerator::assignNonBoolIntoLocation(parsedExp exp, regClass destination){
    buffer.emit(string("sw ") + exp.reg.toString() + string(", (") + destination.toString() + string(")"));
}

void codeGenerator::assignArrayToArray(Id id,parsedExp exp){ //branch bug : if branches will not work due to long distances, this is a good place to adapt the code
    string offsetFromFp = idOffsetFromFp(id);
    regClass reg = regAlloc();
    regClass tempReg = regAlloc();

    for(int i=0; i < id.type.arrayLength; i++){
        int offset = string_to_num(offsetFromFp.c_str()) + i*4; // is it + or - ??
        buffer.emit(string("lw ") + reg.toString() + string(", (") + exp.reg.toString() + string(")"));
        buffer.emit(string("add ") + exp.reg.toString() + string(", ") + exp.reg.toString() + string(", ") + string("4"));
        if(offset >= 0)
            buffer.emit(string("addu ") + tempReg.toString() + string(", $fp, ") + num_to_string(offset));
        else
            buffer.emit(string("subu ") + tempReg.toString() + string(", $fp, ") + num_to_string(-offset));
        buffer.emit(string("sw ") + reg.toString() + string(", (") + tempReg.toString() + string(")"));
    }
    regFree(tempReg);
    regFree(reg);
}

void codeGenerator::assignValueToId(Id id,parsedExp exp) {

    regClass tempReg = regAlloc();
    if(exp.regType == REG_TYPE_REFERENCE) {
        assignArrayToArray(id, exp);
    }
    else {
        if (string_to_num(idOffsetFromFp(id).c_str()) >= 0)
            buffer.emit(string("addu ") + tempReg.toString() + string(", $fp, ") + idOffsetFromFp(id));
        else
            buffer.emit(string("subu ") + tempReg.toString() + string(", $fp, ") +
                        num_to_string(-string_to_num(idOffsetFromFp(id).c_str())));

        if (exp.regType == REG_TYPE_UNDEF) // meaning it is the case of true/false list of bool
            assignBoolIntoLocation(exp, tempReg);
        else
            assignNonBoolIntoLocation(exp, tempReg);
    }
    regFree(tempReg);
}

void codeGenerator::generateArrayOverflowCheck(int arrayLen,regClass reg) {
    if(PRINT_DEBUG)
    cout << "in codeGenerator::generateArrayOverflowCheck(int arrayLen,regClass reg), arrayLen = " << arrayLen << endl;
    string res = "bge ";
    res += reg.toString();
    res += ", ";
    res += num_to_string(arrayLen);
    res += ", ";
    res += "arrayErrHandle"; //code for reporting array access overflow and exit;
    buffer.emit(res);

    res = string("blt ");
    res += reg.toString();
    res += ", $0, ";
    res += "arrayErrHandle";
    buffer.emit(res);

}

void codeGenerator::generateArrayLocationCalc(regClass destenetion, regClass offsetHoldingReg, string offsetOfBaseFromFp){

    regClass tempReg = regAlloc();

    buffer.emit(string("mul ") + tempReg.toString() + string(", ") + offsetHoldingReg.toString() + string(", 4"));

    if(string_to_num(offsetOfBaseFromFp.c_str()) >= 0)
        buffer.emit(string("addu ") + destenetion.toString() + string(", $fp, ") + offsetOfBaseFromFp);
    else
        buffer.emit(string("subu ") + destenetion.toString() + string(", $fp, ") + num_to_string(-string_to_num(offsetOfBaseFromFp.c_str())));
    buffer.emit(string("subu ") + destenetion.toString() + string(", ") + destenetion.toString() + string(", ") + tempReg.toString());

    regFree(tempReg);
}

void codeGenerator::assignValueToArray(Id id,parsedExp offsetExp,parsedExp assignExp){
    regClass addressReg = regAlloc();
    regClass toAssign = regAlloc();

    buffer.emit(string("subu ") + toAssign.toString() + string(", $sp, 4"));
    if (assignExp.regType == REG_TYPE_UNDEF)
        assignBoolIntoLocation(assignExp,toAssign);
    else
        assignNonBoolIntoLocation(assignExp,toAssign);
    buffer.emit(string("lw ") + toAssign.toString() + string(", (") + toAssign.toString() + string(")"));

    generateArrayOverflowCheck(id.type.arrayLength,offsetExp.reg);
    generateArrayLocationCalc(addressReg,offsetExp.reg,idOffsetFromFp(id));

    buffer.emit(string("sw ") + toAssign.toString() + string(", (") + addressReg.toString() + string(")"));

    regFree(toAssign);
    regFree(addressReg);
}

string codeGenerator::trueValueRepresentation(){
    return string("0");
}

string codeGenerator::falseValueRepresentation(){
    return string("1");
}

//============================== Function Handling ==============================/

string codeGenerator::pushStringToDataBuffer(parsedData toPush){
    string toReturn = buffer.genDataLabel();
    buffer.emitData(toReturn + string(": .asciiz \"") + toPush.single_var.name + string("\""));
    return toReturn;
}

int codeGenerator::getIdOffset(string name){
    return scopesList->getId(name).offset;
}

static void pushBool(VarInfo var_bool){
    assert(!(var_bool.trueList.empty() && var_bool.falseList.empty()));
    regClass tempReg = regAlloc();
    vector<int> tempBackPatchList;

    buffer.bpatch(var_bool.trueList,buffer.genLabel());
    buffer.emit(string("add ") + tempReg.toString() + string(", $0, ") + codeGenerator::trueValueRepresentation());
    tempBackPatchList.push_back(buffer.emit(string("j ")));
    buffer.bpatch(var_bool.falseList,buffer.genLabel());
    buffer.emit(string("add ") + tempReg.toString() + string(", $0, ") + codeGenerator::falseValueRepresentation());
    buffer.bpatch(tempBackPatchList,buffer.genLabel());

    buffer.emit(string("subu $sp, $sp, 4"));
    buffer.emit(string("sw ") + tempReg.toString() + string(", ($sp)"));

    regFree(tempReg);
}

static void pushArray(VarInfo var_array){

    regClass tempReg = regAlloc();
    regClass destinationReg = regAlloc();
    buffer.emit(string("subu $sp, $sp, ") + num_to_string(var_array.type.arrayLength*4));
    buffer.emit(string("subu ") + var_array.reg.toString() + string(", ") + num_to_string((var_array.type.arrayLength -1)*4));
    buffer.emit(string("move ") + destinationReg.toString() + string(", $sp"));
    for(int i=0 ; i < var_array.type.arrayLength ; i++){
        buffer.emit(string("lw ") + tempReg.toString() + string(", (") + var_array.reg.toString() + string(")"));
        buffer.emit(string("sw ") + tempReg.toString() + string(", (") + destinationReg.toString() + string(")"));
        buffer.emit(string("addu ") + var_array.reg.toString() + string(", ") + var_array.reg.toString() + string(", 4"));
        buffer.emit(string("addu ") + destinationReg.toString() + string(", ") + destinationReg.toString() + string(", 4"));
    }

    regFree(destinationReg);
    regFree(tempReg);
    regFree(var_array.reg);
}

static void pushNumeric(VarInfo simple_var){
    buffer.emit(string("subu $sp, $sp, 4"));
    buffer.emit(string("sw ") + simple_var.reg.toString() + string(", ($sp)"));
    regFree(simple_var.reg);
}

static void pushString(VarInfo var_string){
    pushNumeric(var_string); // preperation work done in parsing
}

void codeGenerator::pushExpListAndFreeRegisters(parsedExp input_list){
    std::list<VarInfo> inputs = input_list.list_of_vars;
    while (!(inputs.empty())){
        VarInfo temp = inputs.back();
        switch (temp.type.kind){
            case Type::ARRAY :{
                pushArray(temp);
            }break;
            case Type::STRING :{
                pushString(temp);
            }break;
            case Type::BOOL :{
                pushBool(temp);
            }break;
            case Type::INTEGER :{
                pushNumeric(temp);
            }break;
            case Type::BYTE :{
                pushNumeric(temp);
            }break;
            case Type::VOID:
                assert("in codeGenerator::pushExpList(parsedExp input_list), in case: Type::VOID" && 0);      //not supposed to get here
            case Type::UNDEF:
                assert("in codeGenerator::pushExpList(parsedExp input_list), in case: Type::UNDEF" && 0);      //not supposed to get here
        }
        inputs.pop_back();
    }
}

void codeGenerator::callerSaveRegisters(vector<regClass> regsToSave){
    for (int i = 0; i < regsToSave.size() ; i++) {
        //string reg_to_save = IntToReg(i);
        buffer.emit(string("subu $sp, $sp, 4"));
        buffer.emit(string("sw ") + regsToSave[i].toString() + string(", ($sp)"));
    }
    buffer.emit(string("subu $sp, $sp, 4"));
    buffer.emit(string("sw $ra, ($sp)"));
    buffer.emit(string("subu $sp, $sp, 4"));
    buffer.emit(string("sw $fp, ($sp)"));
}

void codeGenerator::callerRestoreRegisters(vector<regClass> regsToSave){
    buffer.emit(string("lw $fp, ($sp)"));
    buffer.emit(string("addu $sp, $sp, 4"));
    buffer.emit(string("lw $ra, ($sp)"));
    buffer.emit(string("addu $sp, $sp, 4"));

    for (int i = regsToSave.size()-1 ; i >= 0 ; i--) {
        //string reg_to_load = IntToReg(i-1);
        buffer.emit(string("lw ") + regsToSave[i].toString() + string(", ($sp)"));
        buffer.emit(string("addu $sp, $sp, 4"));
    }
}

string codeGenerator::IntToReg(int reg_to_save){
    if(reg_to_save <= 17)
        return string("$") + num_to_string(reg_to_save + 8);
    switch (reg_to_save){
        case 18:
            return string("$ra");
        case 19:
            return string("$fp");
    }
}

void codeGenerator::cleanStack(){
    scopes copyOfAllScopes = *scopesList;
    copyOfAllScopes.need_to_print = false;
    Id firstOnStack;
    Id lastOnStack;
    bool foundLast = false;
    int totalOffset;

    while(!copyOfAllScopes.scopesList.empty()){
        if(!copyOfAllScopes.scopesList.front().IdList.empty()) {
            firstOnStack = copyOfAllScopes.scopesList.front().IdList.front();
            copyOfAllScopes.scopesList.front().IdList.pop_front();
            break;
        }
        copyOfAllScopes.scopesList.pop_front();
    }

    cout << "for debug: first Id name = " << firstOnStack.name << ", and offset = " << firstOnStack.offset << endl;

    if(firstOnStack.type.kind == Type::UNDEF) // if there is no variable then stack is already ok
        return;

    totalOffset = firstOnStack.offset + 1; // the plus 1 is because first variable is at location 0, will need to verify it in runtime

    scope lastScope;
    while(!copyOfAllScopes.scopesList.empty()){
        lastScope = copyOfAllScopes.scopesList.front();
        while(!lastScope.IdList.empty()){
            if(!foundLast) {
                foundLast = true;
                lastOnStack = lastScope.IdList.front();
            }
            else{
                if(lastOnStack.offset >= lastScope.IdList.front().offset)
                    lastOnStack = lastScope.IdList.front();
            }
            lastScope.IdList.pop_front();
        }
        copyOfAllScopes.scopesList.pop_front();
    }

    cout << "for debug: last Id name = " << lastOnStack.name << ", and offset = " << lastOnStack.offset << endl;

    if(lastOnStack.type.kind != Type::UNDEF)
        totalOffset -= lastOnStack.offset;

    totalOffset *= 4;
    buffer.emit(string("add $sp, $sp, ") + num_to_string(totalOffset));
}

void codeGenerator::returnVoidFunction(){
    cleanStack();
    buffer.emit(string("jr $ra"));
}

void codeGenerator::returnFunction(parsedExp returnExp){

    regClass tempReg = regAlloc();
    buffer.emit(string("subu ") + tempReg.toString() + string(", $sp, 4"));
    if(returnExp.regType == REG_TYPE_UNDEF)
        assignBoolIntoLocation(returnExp, tempReg);
    else {
        assignNonBoolIntoLocation(returnExp, tempReg);
        regFree(returnExp.reg);
    }
    buffer.emit(string("lw $v0, -4($sp)"));
    cleanStack();
    buffer.emit(string("jr $ra"));

    regFree(tempReg);
}

void codeGenerator::callFunction(string func_name, parsedExp input_list,regClass returnReg){
    buffer.emit(string("# Begin of calling to function code"));

    vector<regClass> usedRegisters;
    usedRegisters = getAllUsedRegisters();
    callerSaveRegisters(usedRegisters);

    pushExpListAndFreeRegisters(input_list);

    // creating a new frame for the calley function
    buffer.emit(string("subu $fp, $sp, 4"));

    // inserting to $ra the return address
    vector<int> returnBackPatchList;
    returnBackPatchList.push_back(buffer.emit(string("la $ra, ")));

    // jump to function
    buffer.emit(string("jal "+func_name));

    // backPatching the return address
    buffer.bpatch(returnBackPatchList,buffer.genLabel());

    // restoring registers (includes $fp)
    // assumes the calley function cleaning its own variables, include the passed parameters
    callerRestoreRegisters(usedRegisters);

    // saving the return value
    buffer.emit(string("add ") + returnReg.toString() + string(", $0, $v0"));
    buffer.emit(string("# End of calling to function code"));
}

void codeGenerator::putIdAddressInRegister(Id id, regClass resultReg){
    if(string_to_num(idOffsetFromFp(id).c_str()) < 0){
        buffer.emit(string("subu ") + resultReg.toString() + string(", $fp, ") + num_to_string(-string_to_num(idOffsetFromFp(id).c_str())));
    }
    else{
        buffer.emit(string("addu ") + resultReg.toString() + string(", $fp, ") + (idOffsetFromFp(id)));
    }
}

void codeGenerator::generateNewSingleVarCreation(parsedExp exp){
    buffer.emit(string("subu $sp, $sp, 4"));
    string s1 = "sw ";
    if(exp.single_var.type.kind == Type::BOOL)
        s1 += codeGenerator::falseValueRepresentation();
    else
        s1 += string("$0");
    s1 += string(", ($sp)");
    buffer.emit(s1);
}
