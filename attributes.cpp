#include "attributes.h"

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
VarInfo::VarInfo() : value(0){};
VarInfo::VarInfo(Type type): value(0),VarBase(type) {};
VarInfo::VarInfo(int val) : value(val), VarBase(string(),Type::INTEGER) {};
VarInfo::VarInfo(int val, string str, Type::typeKind kind):
            value(val), VarBase(str,kind){};
VarInfo::VarInfo(int val, string str, Type::typeKind kind, int len):
            value(val), VarBase(str, kind, len){};

//============================= ID CLASS ====================================//
Id::Id(Type type_t, int offset, string name_t) : offset(offset){
        this->type = type_t;
        this->name = name_t;
    };

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
//=========================== ParsedExp Class ===============================//
parsedExp::parsedExp(Type::typeKind kind): parsedData(Type(kind)){}
parsedExp::parsedExp(Type type) : parsedData(type){};
parsedExp::parsedExp(parsedExp exp, binOps ops) {
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
parsedExp::parsedExp(parsedExp exp1, parsedExp exp2, binOps ops){
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
parsedExp::parsedExp(parsedData data) : parsedData(data){};
parsedExp::parsedExp(parsedData data1, parsedData data2, binOps ops){
        *this = parsedExp(parsedExp(data1), parsedExp(data2), ops);
    }
parsedExp::parsedExp(parsedData data, binOps ops){
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
    // HW5 addition
    string var_offset = \
        std::to_string(scopesList.getId(Id.single_var.name).offset*4*newType.arrayLength);
    buffer.emit(string("subu $sp, $sp, "+var_offset));
    // HW5 addition
}
void scopes::addIdNotArray(parsedData type) {
    if (containsIdName(type.single_var.name) || containsFunctionName(type.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_DEF, type.single_var.name);
    }//throw {/* appropriate exception*/};

    scopesList.front().addId(type.single_var.type, type.single_var.name);
    if (PRINT_DEBUG) cout << "\n\n\nadded ID (not array): " << type.single_var.name << endl;

    // HW5 addition
    string var_offset = std::to_string(scopesList.getId(type.single_var.name).offset*4);
    buffer.emit(string("subu $sp, $sp, "+var_offset));
    // HW5 addition
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
void scopes::verifyAssign(parsedData Id,parsedData exp) {
    if (!containsIdName(Id.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF_FUN, Id.single_var.name);
    }//throw {/*  appropriate exception */}; // id not found

    //HW5 addition
    Id id = getId(Id.single_var.name);
    //HW5 addition
    Type idType = id.type;
    if (!(idType == exp.single_var.type) && // the types are different, and it is not a case of assign byte to int
        !((idType.kind == Type::INTEGER) && (exp.single_var.type.kind == Type::BYTE))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; // incompatible types

    //HW5 addition
    string reg_to_sw = std::to_string(exp.reg);     //TODO add register to expression
    string offset_str = std::to_string(id.offset*4);
    string to_emit = (id.offset == 0 ?                                  \
                            string("sw $"+reg_to_sw+", ($sp)") :        \
                            string("sw $"+reg_to_sw+", "+offset_str+"($sp)" ));
    buffer.emit(to_emit);
    //HW5 addition
}
void scopes::verifyAssignToArray(parsedData idInput, parsedExp arrIndex, parsedData assigned) {

    if (!containsIdName(idInput.single_var.name)) {
        throw parsingExceptions(parsingExceptions::ERR_UNDEF, idInput.single_var.name);
    }//throw {/*  appropriate exception */}; //id not found

    Id id = getId(idInput.single_var.name);
    Type idType = id.type;
    Type indexType = arrIndex.single_var.type;
    Type assignedType = assigned.single_var.type;

    if (!(idType.kind == Type::ARRAY)) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; // id is not array

    if ((!(indexType.kind == Type::BYTE)) &&
        (!(indexType.kind == Type::INTEGER))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; // array index is not of numeric type

    if (!(idType.arrayType == assignedType.kind) &&
        // the types are different, and it is not a case of assign byte to int
        !((idType.arrayType == idType.INTEGER) && (assignedType.kind == assignedType.BYTE))) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; // array type is not compatible with exp type

    //HW5 addition
    string reg_to_sw = std::to_string(assigned.reg);     //TODO add register to expression
    string offset_str = std::to_string(id.offset*4+arrIndex.single_var.value*4);
    string to_emit = (id.offset == 0 ?                                  \
                            string("sw $"+reg_to_sw+", ($sp)") :        \
                            string("sw $"+reg_to_sw+", "+offset_str+"($sp)" ));
    buffer.emit(to_emit);
    //HW5 addition

}
void scopes::verifyReturnTypeVoid() {
    if (!(functions.front().return_type.kind == Type::VOID)) {
        throw parsingExceptions(parsingExceptions::ERR_MISMATCH);
    }//throw {/*  appropriate exception */}; //function return different type
}
void scopes::verifyReturnType(parsedData returnType) {
    if (PRINT_DEBUG) cout << "in verifyReturnType " << endl;
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


