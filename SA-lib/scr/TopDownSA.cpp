//
// Created by Admin on 01.04.2024.
//

#include <TopDownSA.h>

LL::LL(Lexer &lexer, const std::string & outPathstr): lexer(lexer){
    outPath = outPathstr;
}

void LL::solve() {
    setLexem();
    statesIt = states.begin();
    nextState(0);
    addString("StmtList");

    bool f = StmtList() and *iter == LEX_EOF;
    outStr.open(outPath);
    if (f) {
        for (const auto &now: output) {
            outStr << now << endl;
            //cout << now << endl;
        }
        outStr << endl << "Result: Accept" << endl;
    }
    else outStr << endl << "Result: Error" << endl;

    outStr.close();
    return;
}
//
// Other func

void LL::setLexem() {
    if (list.empty() or iter == list.end() - 1){
        list.emplace_back(lexer.getNextLexem());
        iter = list.end() - 1;
        cout << iter->first << " " << iter->second << endl;

        return;
    }
    iter++;
    cout << iter->first << " " << iter->second << endl;
    return;
}



void LL::nextState(const int &state) {
    states.push_back(state);
    statesIt = states.end() - 1;
}

void LL::backStateIt() {
    states.pop_back();
    if (statesIt > states.begin()) statesIt--;
    else statesIt = states.begin();
}

void LL::addString(const std::string &str) {
    string outputString;
    treeHeight++;
    for (auto i = states.begin(); i != states.end(); i++){
        if (i == states.end() - 1){
            if (*i == 1) outputString += "├";
            else outputString += "└";
            break;
        }
        if (*i == 1) outputString += "│ ";
        else outputString += "  ";
    }
    output.push_back(outputString + str);
    //cout << outputString + str << endl;
}
//semantic func
string LL::newLabel() {
    string label = "L" + to_string(labelCounter);
    labelCounter++;
    return label;
}

string LL::alloc(const string &scope) {
    codeCounter++;
    table.push_back({to_string(codeCounter), "$T" + to_string(tempVarCounter), "var"});
    return to_string(codeCounter);
}

string LL::addVar(const std::string &name, const string &scope, const std::string type, const std::string &init) {
    bool fl = false;
    for (auto now: table) {
        if (now.name == name and now.scope == scope){
            fl = true;
            break;
        }
    }
    if (fl) return "$Error";
    codeCounter++;
    table.push_back({to_string(codeCounter), name, "var", type, "-1", init, scope});
    return to_string(codeCounter);
}

string LL::addFunc(const std::string &name, const std::string &type) {
    bool fl = false;
    for (auto now: table){
        if (now.name == name){
            fl = true;
            break;
        }
    }
    if (fl) return "$Error";
    codeCounter++;
    table.push_back({to_string(codeCounter), name, "func", type});
    return to_string(codeCounter);
}

string LL::checkVar(const std::string &scope, const std::string &name) {
    string tempCode = "$Error";
    for (auto  now: table){
        if ((now.scope == scope or now.scope == "-1") and now.name == name and now.kind == "var"){
            if (now.scope == scope){
                tempCode = now.code;
                break;
            }
            tempCode = now.code;
        }
    }
    return tempCode;
}

string LL::checkFunc(const std::string &name, const std::string &len) {
    for (auto now: table){
        if (now.scope == "-1" and now.name == name and now.kind == "func" and now.len == len)return now.code;
    }
    return "$Error";
}
//
//Expression Grammer
bool LL::E() {
    nextState(0);
    addString("E7");

    if (!E7()) {return false; }
    backStateIt();
    return true;
}

bool LL::E7(){

    nextState(1);
    addString("E6");

    if (!E6()) {return false; }

    nextState(0);
    addString("E7\'");
    if (!E7list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E7list(){
    if (iter->first == "opor"){
        setLexem();

        nextState(1);
        addString("opor E6");

        if (!E6()) {return false; }
        nextState(0);
        addString("E7\'");
        if (!E7list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E6(){
    nextState(1);
    addString("E5");

    if (!E5()) {return false; }

    nextState(0);
    addString("E6\'");

    if (!E6list()) {return false;}
    backStateIt();
    return true;
}

bool LL::E6list(){
    if (iter->first == "opand"){
        setLexem();

        nextState(1);
        addString("opand E5");

        if (!E5()) {return false; }

        nextState(0);
        addString("E6\'");

        if (!E6list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E5(){

    nextState(1);
    addString("E4");

    if (!E4()) {return false; }

    nextState(0);
    addString("E5\'");

    if (!E5list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E5list(){
    if (iter->first == "opeq" or iter->first == "opne" or iter->first == "oplt" or iter->first == "opgt" or
            iter->first == "ople"){
        nextState(0);
        addString(iter->first + " E4");
        setLexem();

        if (!E4()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E4(){

    nextState(1);
    addString("E3");

    if (!E3()) {return false; }

    nextState(0);
    addString("E4\'");

    if (!E4list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E4list(){
    if (iter->first == "opplus" or iter->first == "opminus"){
        nextState(1);
        addString(iter->first + " E3");
        setLexem();


        if (!E3()) {return false; }

        nextState(0);
        addString("E4\'");

        if (!E4list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E3(){

    nextState(1);
    addString("E2");

    if (!E2()) {return false; }

    nextState(0);
    addString("E3\'");

    if (!E3list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E3list(){
    if (iter->first == "opmul"){
        setLexem();

        nextState(1);
        addString("opmul E2");

        if (!E2()) {return false; }

        nextState(0);
        addString("E3\'");

        if (!E3list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E2(){
    if (iter->first == "opnot"){
        setLexem();
        nextState(0);
        addString("opnot E1");

        if (!E1()){return false;}
        backStateIt();
        return true;
    }
    else{
        nextState(0);
        addString("E1");

        if (!E1()){return false;}
    }
    backStateIt();
    return true;
}

bool LL::E1(){
    if (iter->first == "opinc"){
        setLexem();

        if (iter->first != "id") {return false; }
        nextState(0);
        addString("opinc " + iter->second);
        backStateIt();
        backStateIt();

        setLexem();
        return true;
    }
    else if (iter->first == "num" or iter->first == "char"){
        nextState(0);
        addString(iter->second);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    else if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar E");

        if (!E()) {return false; }

        nextState(0);
        addString("rpar");
        if (iter->first != "rpar") {return false; }
        backStateIt();
        backStateIt();
        setLexem();
        return true;
    }
    else if (iter->first == "id"){
        nextState(0);
        addString(iter->second + " E1\'");
        setLexem();
        if (!E1List()) {return false; }
        backStateIt();
        return true;
    }

    return false;
}

bool LL::E1List() {
    if (iter->first == "lpar"){
        setLexem();

        nextState(1);
        addString("lpar Arglist");

        if (!Arglist()) {return false; }

        nextState(0);
        addString("rpar");
        if (iter->first != "rpar") {return false; }
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    else if (iter->first == "opinc") {
        setLexem();

        nextState(0);
        addString("opinc");
        backStateIt();
        backStateIt();
    }
    backStateIt();
    return true;
}

//Entry point
bool LL::StmtList() {
    if (*iter == LEX_EOF) return true;

    nextState(1);
    addString("Stmt");
    auto listIt = iter;
    if (!Stmt()){
        iter = listIt;
        backStateIt();
        backStateIt();
        output.pop_back();
        return true;
    }
    nextState(0);
    addString("StmtList");
    if (!StmtList()) return false;
    backStateIt();
    return true;
}

bool LL::Stmt() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "kwint" or iter->first == "kwchar"){
        nextState(0);
        addString("DeclareStmt");

        if (!DeclareStmt()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "id"){
        nextState(0);
        addString("AssignOrCallOp");

        if (!AssignOrCallOp()) return false;

        backStateIt();
        return true;
    }

    if (iter->first == "kwwhile"){
        nextState(0);
        addString("kwwhile WhileOp");

        if (!WhileOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwfor"){
        nextState(0);
        addString("kwfor ForOp");
        if (!ForOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwif"){
        nextState(0);
        addString("kwif IfOp");
        if (!IfOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwswitch"){
        nextState(0);
        addString("kwswitch SwitchOp");
        if (!SwitchOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwin"){
        nextState(0);
        addString("kwin IOp");
        if (!IOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwout"){
        nextState(0);
        addString("kwout OOp");
        if (!OOp()) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "semicolon"){
        setLexem();
        nextState(0);
        addString("semicolon");
        backStateIt();
        backStateIt();
        return true;
    }


    if (iter->first == "lbrace"){
        setLexem();
        nextState(1);
        addString("lbrace StmtList");
        if (!StmtList()) {return false; }
//        cout << "LL" << iter->second << endl;
        if (iter->first != "rbrace") {return false; }
        setLexem();
        nextState(0);
        addString("rbrace");
        backStateIt();
        backStateIt();
        return true;
    }

    if (iter->first == "kwreturn"){
        setLexem();

        nextState(1);
        addString("kwreturn E");

        if (!E()) {return false; }
        if (iter->first != "semicolon") {return false; }

        nextState(0);
        addString("semicolon");
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }

    return false;
}
//
//Declare block
bool LL::Type() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "kwint" or iter->first == "kwchar"){
        nextState(0);
        addString(iter->first);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    return false;
}

bool LL::DeclareStmt() {
    if (*iter == LEX_EOF) return false;

    nextState(1);
    addString("Type");
    if (!Type()) {return false; }
    if (iter->first != "id") return false;
    nextState(0);
    addString(" " + iter->second + " DeclareStmt'");
    setLexem();
    if (!DeclareStmtList()) return false;
    backStateIt();
    return true;

}

bool LL::DeclareStmtList() {
    if (*iter == LEX_EOF) return false;
    if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar ParamList");
        if (!ParamList()) return false;
        if (iter->first != "rpar") return false;
        setLexem();
        if (iter->first != "lbrace") return false;
        setLexem();
        nextState(1);
        addString("rpar lbrace StmtList");
        if (!StmtList()) return false;
        if (iter->first != "rbrace") return false;
        setLexem();
        nextState(0);
        addString("rbrace");
        backStateIt();
        backStateIt();
        return true;
    }
    else if (iter->first == "opassign"){
        setLexem();

        if (iter->first == "num"){
            nextState(1);
            addString("opassign " + iter->second + " DeclareVarList");
            setLexem();
            if(!DeclareVarList()) return false;
            if (iter->first != "semicolon") return false;
            setLexem();
            nextState(0);
            addString("semicolon");
            backStateIt();
            backStateIt();
            return true;
        }

        else if (iter->first == "char"){
            nextState(1);
            addString("opassign " + iter->second + " DeclareVarList");
            setLexem();
            if(!DeclareVarList()) return false;
            if (iter->first != "semicolon") return false;
            setLexem();
            nextState(0);
            addString("semicolon");
            backStateIt();
            backStateIt();
            return true;
        }
        else return false;
    }
    else {
        nextState(1);
        addString("DeclareVarList");
        if (!DeclareVarList()) return false;
        if (iter->first != "semicolon") return false;
        setLexem();
        nextState(0);
        addString("semicolon");
        backStateIt();
        backStateIt();
        return true;
    }
}

bool LL::DeclareVarList() {
    if (*iter == LEX_EOF) return false;
    //cout << "!!" << endl;

    if (iter->first == "comma") {
        setLexem();

        if (iter->first != "id") return false;
        nextState(1);
        addString("comma " + iter->second + " InitVar");
        setLexem();
        if (!InitVar()) return false;
        nextState(0);
        addString("DeclareVarList");
        if (!DeclareVarList()) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}

bool LL::InitVar() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "opassign"){
        setLexem();

        if (iter->first != "num" and iter->first != "char") return false;
        nextState(0);
        addString("opassign " + iter->second);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}

bool LL::ParamList() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "kwint" or iter->first == "kwchar"){
        nextState(1);
        addString("Type");
        if (iter->first != "id") return false;
        nextState(0);
        addString(" " + iter->second + " ParamList'");
        setLexem();
        if (!ParamListList()) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}

bool LL::ParamListList() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "comma"){
        setLexem();
        if (iter->first != "kwint" and iter->first != "kwchar") {
            backStateIt();
            return true; }
        nextState(1);
        addString("comma Type");
        if (iter->first != "id") return false;
        nextState(0);
        addString(" " + iter->second + " ParamList'");
        setLexem();
        if (!ParamListList()) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}
//
// ArgList
bool LL::Arglist() {
    //if (iter == list.end()) cout << "!!!!!!!!!!!!!!!!" << endl;
    if (iter->first != "num" and iter->first != "char" and iter->first != "id" and iter->first != "opinc" and iter->first != "lpar"){
        backStateIt();
        return true;
    }
    nextState(1);
    addString(iter->first + " E");
    if (!E()) return false;
    nextState(0);
    addString("ArgList'");
    if(!ArgListList()) return false;
    backStateIt();
    return true;
}

bool LL::ArgListList() {
    if (iter->first == "comma"){
        setLexem();
        if (iter->first != "num" and iter->first != "char" and iter->first != "id" and iter->first != "opinc" and iter->first != "lpar") return false;
        nextState(1);
        addString("comma E");

        if (!E()) { return false;}
        nextState(0);
        addString("Arglist'");
        if (!ArgListList()) return false;
    }
    backStateIt();
    return true;
}
//
// Assign Or Call operation block
bool LL::AssignOrCallOp() {
    nextState(1);
    addString("AssignOrCall");
    if (!AssignOrCall()) return false;
    if (iter->first != "semicolon") return false;
    nextState(0);
    addString("semicolon");
    setLexem();
    backStateIt();
    backStateIt();
    return true;
}

bool LL::AssignOrCall() {
    if (iter->first != "id") return false;
    nextState(0);
    addString(iter->second + " AssignOrCall'");
    setLexem();
    if (!AssignOrCallList()) return false;
    backStateIt();
    return true;
}

bool LL::AssignOrCallList() {
    if (iter->first == "opassign"){
        setLexem();
        nextState(0);
        addString("opassign E");
        if (!E()) return false;
        backStateIt();
        return true;
    }
    else if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar Arglist");
        if (!Arglist()) return false;
        if (iter->first != "rpar") return false;
        nextState(0);
        addString("rpar");
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    return false;
}//
//while block
bool LL::WhileOp() {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");
    if (!E()) return false;
    if (iter->first != "rpar") return false;
    setLexem();
    nextState(0);
    addString("rpar Stmt");
    if (!Stmt()) return false;
    backStateIt();
    return true;
}
//
// block "ForOp"
bool LL::ForOp() {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar ForInit");
    if (!ForInit()) return false;
    if (iter->first != "semicolon") return false;
    setLexem();
    backStateIt();
    nextState(1);
    addString("semicolon ForExp");
    if (!ForExp()) return false;
    if (iter->first != "semicolon") return false;
    setLexem();
    backStateIt();
    nextState(1);
    addString("semicolon ForLoop");
    if (!ForLoop()) return false;

    nextState(0);
    addString("rpar Stmt");
    if (!Stmt()) {return false; }
    backStateIt();
    return true;
}

bool LL::ForInit() {
    if (iter->second == "id"){
        nextState(0);
        addString("AssignOrCall");
        if (!AssignOrCall()) return false;
    }
    return true;
}

bool LL::ForExp() {
    if (iter->first == "lpar" or iter->first == "id" or iter->first == "num" or iter->first == "char" or iter->first == "opinc"){
        nextState(0);
        addString("E");

        if (!E()) { return false;}
    }
    return true;
}

bool LL::ForLoop() {
    if (iter->first == "opinc"){
        setLexem();

        if (iter->first != "id") return false;
        auto item = iter->second;
        setLexem();
        if (iter->first != "rpar") return false;
        nextState(0);
        addString("opinc " + item);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    if (iter->first == "id"){
        nextState(1);
        addString("AssignOrCall");
        if (!AssignOrCall()) return false;
        if (iter->first != "rpar") return false;
        setLexem();
        backStateIt();
        return true;
    }
    if (iter->first != "rpar") return false;
    setLexem();
    backStateIt();
    return true;
}
//
// block "IfOp"
bool LL::IfOp() {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");
    if (!E()) return false;
    if (iter->first != "rpar") return false;
    setLexem();
    nextState(1);
    addString("rpar Stmt");
    if (!Stmt()) return false;
    nextState(0);
    addString("ElsePart");
    if (iter->first == "kwelse"){
        if (!ElsePart()) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    backStateIt();
    return true;
}

bool LL::ElsePart() {
    nextState(0);
    addString("kwelse Stmt");
    setLexem();
    if(!Stmt()) return false;
    backStateIt();
    return true;
}
//
// block "SwitchOp"
bool LL::SwitchOp() {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");
    if (!E()) return false;
    if (iter->first != "rpar") return false;
    setLexem();

    if (iter->first != "lbrace") return false;
    setLexem();
    nextState(1);
    addString("lpar lbrace Cases");

    if (!Cases()) return false;
    //cout << 1 << endl;
    if (iter->first != "rbrace") return false;
    setLexem();
    nextState(0);
    addString("rbrace");
    backStateIt();
    backStateIt();
    return true;
}

bool LL::Cases() {
    nextState( 1);
    addString("Acase");
    if (!ACase()) return false;
    //cout << 2 << endl;
    nextState(0);
    addString("CasesList");
    if (!CasesList()) return false;
    backStateIt();
    return true;
}

bool LL::CasesList() {
    if (iter->first == "kwdefault" or iter->first == "kwcase") {
        nextState(1);
        addString("Acase");
        if (ACase()) return false;
        nextState(0);
        addString("CasesList");
        if (!CasesList()) return false;

    }
    backStateIt();
    return true;
}

bool LL::ACase() {
    if (iter->first == "kwcase"){
        setLexem();
        if (iter->first != "num" and iter->first != "char")return false;
        auto item = iter->second;
        setLexem();
        if (iter->first != "colon") return false;
        nextState(0);
        addString("kwcase " + item + " colon StmtList");
        setLexem();
        if (!StmtList()) return false;
        backStateIt();
        return true;

    }
    else if (iter->first == "kwdefault"){
        setLexem();

        if (iter->first != "colon") return false;
        setLexem();
        nextState(0);
        addString("kwdefault colon StmtList");
        if (!StmtList()) return false;
        backStateIt();
        return true;
    }
    return false;
}
//
// block "IN" and "OUT"
bool LL::IOp() {
    setLexem();
    if (iter->first != "id") return false;
    auto item = iter->second;
    setLexem();

    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(0);
    addString(item + "semicolon");
    backStateIt();
    backStateIt();
    return true;
}

bool LL::OOp() {
    setLexem();

    nextState(1);
    addString("kwout OOp'");
    if (!OOpList()) return false;
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(0);
    addString("semicolon");
    backStateIt();
    backStateIt();
    return true;
}

bool LL::OOpList() {
    if (iter->first == "str"){
        nextState(0);
        addString("\"" + iter->second + "\"");
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    nextState(0);
    addString("E");
    if (!E()) return false;
    backStateIt();
    return true;
}