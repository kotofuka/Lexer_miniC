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
    for (const auto& now: output){
        outStr << now << endl;
        cout<< now << endl;
    }

    outStr.close();
    return;
}
//
// Other func

void LL::setLexem() {
    if (list.empty() or iter == list.end() - 1){
        list.emplace_back(lexer.getNextLexem());
        iter = list.end() - 1;
        //cout << iter->first << " " << iter->second << endl;

        return;
    }
    iter++;
    //cout << iter->first << " " << iter->second << endl;
    return;
}

void LL::nextState(const int &state) {
    states.emplace_back(state);
    statesIt = states.end() - 1;
}

void LL::backStateIt() {
    states.pop_back();
    statesIt = states.end() - 1;
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
    cout << outputString + str << endl;
}
//
//Expression Grammer
bool LL::E() {
    auto tempIt = statesIt;

    nextState(0);
    addString("E7");

    if (!E7()) {return false; }
    backStateIt();
    return true;
}

bool LL::E7(){
    auto  tempIt = statesIt;

    nextState(1);
    addString("E6");

    if (!E6()) {return false; }
    tempIt = statesIt;

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
        auto tempIt = statesIt;

        if (!E6()) {return false; }
        tempIt = statesIt;
        nextState(0);
        addString("E7\'");
        if (!E7list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E6(){
    auto tempIt = statesIt;

    nextState(1);
    addString("E5");

    if (!E5()) {return false; }

    tempIt = statesIt;
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
        auto tempIt = statesIt;

        if (!E5()) {return false; }

        tempIt = statesIt;
        nextState(0);
        addString("E6\'");

        if (!E6list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E5(){
    auto tempIt = statesIt;

    nextState(1);
    addString("E4");

    if (!E4()) {return false; }

    tempIt = statesIt;
    nextState(0);
    addString("E5\'");

    if (!E5list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E5list(){
    if (iter->first == "opeq" or iter->first == "opne" or iter->first == "oplt" or iter->first == "opgt" or
            iter->first == "ople"){
        string opToken = iter->first;
        setLexem();

        nextState(0);
        addString(opToken + " E4");
        auto tempIt = statesIt;

        if (!E4()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E4(){
    auto tempIt = statesIt;

    nextState(1);
    addString("E3");

    if (!E3()) {return false; }

    tempIt = statesIt;
    nextState(0);
    addString("E4\'");

    if (!E4list()) {return false; }
    backStateIt();
    return true;
}

bool LL::E4list(){
    if (iter->first == "opplus" or iter->first == "opminus"){
        string opToken = iter->first;
        setLexem();

        nextState(1);
        addString(opToken + " E3");
        auto tempIt = statesIt;

        if (!E3()) {return false; }

        tempIt = statesIt;
        nextState(0);
        addString("E4\'");

        if (!E4list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E3(){
    auto tempIt = statesIt;

    nextState(1);
    addString("E2");

    if (!E2()) {return false; }

    tempIt = statesIt;
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
        auto tempIt = statesIt;

        if (!E2()) {return false; }

        nextState(0);
        addString("E3\'");

        if (!E3list()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::E2(){
    auto tempIt = statesIt;
    if (iter->first == "opnot"){
        setLexem();
        nextState(0);
        addString("opnot E1");

        if (!E1()){return false;}
        backStateIt();
    }
    else{
        nextState(0);
        addString("E1");

        if (!E1()){return false;}
        backStateIt();
    }
    backStateIt();
    return true;
}

bool LL::E1(){
    auto tempIt = statesIt;

    if (iter->first == "opinc"){
        setLexem();

        nextState(1);
        addString("opinc");
        backStateIt();

        nextState(0);

        if (iter->first != "id") {return false; }
        addString(iter->second);
        backStateIt();

        setLexem();
        return true;
    }
    else if (iter->first == "num"){
        nextState(0);
        addString(iter->second);
        setLexem();
        backStateIt();
        return true;
    }
    else if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar E");

        if (!E()) {return false; }

        nextState(0);

        if (iter->first != "rpar") {return false; }
        addString("rpar");
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
    auto tempIt = statesIt;
    if (iter->first == "lpar"){
        setLexem();

        nextState(1);
        addString("lpar Arglist");

        if (!Arglist()) {return false; }

        tempIt = statesIt;
        nextState(0);

        if (iter->first != "rpar") {return false; }
        addString("rpar");
        setLexem();
        backStateIt();


    }
    else if (iter->first == "opinc") {
        setLexem();

        nextState(0);
        addString("opinc");
        backStateIt();

    }
    return true;
}

//Entry point
bool LL::StmtList() {
    auto tempIt = statesIt;
    if (*iter == LEX_EOF) return true;

    vector<Lexem>::iterator it = iter;
    int tempHeight = treeHeight;
    nextState(1);
    addString("Stmt");
    if (Stmt()) {
        tempHeight = treeHeight;
        nextState(0);
        addString("StmtList");
        if (!StmtList()) return false;
    } else {
        iter = it;
        backStateIt();
        output.pop_back();
    }
    backStateIt();
    return true;
}

bool LL::Stmt() {
    if (*iter == LEX_EOF) return false;

    vector<Lexem>::iterator tempIt = iter;
    int tempHeight = treeHeight;

    if (DeclareStmt()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
    }
    tempHeight = treeHeight;

    if (AssignOrCallOp()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
    }

    tempHeight = treeHeight;

    if (WhileOp()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
        backStateIt();
    }

    tempHeight = treeHeight;

    if (ForOp()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
    }

    tempHeight = treeHeight;

    if (IfOp()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
    }

    tempHeight = treeHeight;

    if (SwitchOp()) return true;
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();
        treeHeight = tempHeight;
    }

    tempHeight = treeHeight;

    if (IOp()) return true;
    else {
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        treeHeight = tempHeight;
        iter = tempIt;
    }

    if (OOp()) return true;
    else {
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        iter = tempIt;
        treeHeight = tempHeight;
    }

    tempHeight = treeHeight;

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
        tempHeight = treeHeight;
        if (!StmtList()) return false;
//        cout << "LL" << iter->second << endl;
        if (iter->first != "rbrace") return false;
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

        if (!E()) return false;
        if (iter->first != "semicolon") return false;

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
        return true;
    }
    backStateIt();
    return false;
}

bool LL::DeclareStmt() {
    if (*iter == LEX_EOF) return false;

    nextState(0);
    addString("DeclareStmt");

    nextState(1);
    addString("Type");
    if (!Type()) return false;
    backStateIt();
    if (iter->first != "id") return false;
    nextState(0);
    addString(" " + iter->second + " DeclareStmt'");
    setLexem();
    if (!DeclareStmtList()) return false;
    backStateIt();
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
        nextState(1);
        addString("rpar");
        backStateIt();
        if (iter->first != "lbrace") return false;
        setLexem();
        nextState(1);
        addString("lbrace StmtList");
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
        nextState(1);
        addString("opassign");
        backStateIt();
        if (iter->first == "num"){
            setLexem();
            nextState(1);
            addString(" " + iter->second + " DeclareVarList");
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
            setLexem();
            nextState(1);
            addString(" " + iter->second + " DeclareVarList");
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
        nextState(1);
        addString("comma");
        backStateIt();
        if (iter->first != "id") return false;
        nextState(1);
        addString(" " + iter->second + " InitVar");
        setLexem();
        if (!InitVar()) return false;
        backStateIt();
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
        nextState(1);
        addString("opassign");
        backStateIt();
        if (iter->first != "num" and iter->first != "char") return false;
        nextState(0);
        addString(" " + iter->second);
        setLexem();
        backStateIt();
        return true;
    }
    return true;
}

bool LL::ParamList() {
    if (*iter == LEX_EOF) return false;

    vector<Lexem>::iterator tempIt = iter;
    auto tempHeight = treeHeight;
    nextState(1);
    addString("Type");
    if (Type()){
        if (iter->first != "id") return false;
        backStateIt();
        nextState(0);
        addString(" " + iter->second + " ParamList'");
        setLexem();
        if (!ParamListList()) return false;
        backStateIt();
        return true;
    }
    else {
        iter = tempIt;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
    }
    backStateIt();
    return true;
}

bool LL::ParamListList() {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "comma"){
        setLexem();
        nextState(1);
        addString("comma Type");
        if (!Type()) return false;
        backStateIt();
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
    if (iter->first == "id"){
        nextState(0);
        addString(iter->second + " ArgList'");
        //cout << iter->first << iter->second << endl;

        setLexem();
        auto tempIt = statesIt;
        if (!ArgListList()) {return false; }
    }
    backStateIt();
    return true;
}

bool LL::ArgListList() {
    if (iter->first == "comma"){
        auto tempIt = statesIt;

        nextState(1);
        addString("comma");
        setLexem();
        backStateIt();
        nextState(0);
        if (iter->first != "id") {return false; }
        addString(iter->second + " ArgList'");
        setLexem();
        tempIt = statesIt;
        if (!ArgListList()) {return false; }
    }
    backStateIt();
    return true;
}
//
// Assign Or Call operation block
bool LL::AssignOrCallOp() {
    nextState(0);
    addString("AssignOrCallOp");
    nextState(1);
    addString("AssignOrCall");
    if (!AssignOrCall()) return false;
    if (iter->first != "semicolon") return false;
    nextState(0);
    addString("semicolon");
    setLexem();
    backStateIt();
    backStateIt();
    backStateIt();
    return true;
}

bool LL::AssignOrCall() {
    if (iter->first != "id") return false;
    nextState(0);
    addString(iter->second + " AssignOrCall' ");
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
    nextState(0);
    addString("WhileOp");
    if (iter->first != "kwwhile") return false;
    setLexem();
    nextState(1);
    addString("kwwhile");
    backStateIt();
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
    backStateIt();
    backStateIt();
    return true;
}
//
// block "ForOp"
bool LL::ForOp() {
    nextState(0);
    addString("ForOp");

    if (iter->first != "kwfor") return false;
    setLexem();
    nextState(1);
    addString("kwfor");
    backStateIt();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar ForInit");
    ForInit();
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(1);
    addString("semicolon ForExp");
    ForExp();
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(1);
    addString("semicolon ForLoop");
    nextState(0);
    if (!ForLoop()) return false;
    backStateIt();
    if (iter->first != "rpar") return false;
    setLexem();
    nextState(0);
    addString("rpar Stmt");
    int tempHeight = treeHeight;

    if (!Stmt()) {
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        return false; }
    backStateIt();
    backStateIt();
    return true;
}

bool LL::ForInit() {
    vector<Lexem>::iterator it = iter;
    int tempHeight = treeHeight;

    nextState(0);
    addString("AssignOrCall");
    if (!AssignOrCall()) {
        iter = it;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        backStateIt();

    }
    backStateIt();
    return true;
}

bool LL::ForExp() {
    vector<Lexem>::iterator it = iter;
    int tempHeight = treeHeight;

    nextState(0);
    addString("E");

    if (!E()) {
        iter = it;
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        for (int i = 0; i < treeHeight - tempHeight; i++) backStateIt();
        treeHeight = tempHeight;
    }
    backStateIt();
    return true;
}

bool LL::ForLoop() {
    if (iter->first == "opinc"){
        setLexem();
        nextState(1);
        addString("opinc");
        backStateIt();
        if (iter->first != "id") return false;
        nextState(0);
        addString(iter->second);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    vector<Lexem>::iterator it = iter;
    if (!AssignOrCall()) iter = it;
    backStateIt();
    return true;
}
//
// block "IfOp"
bool LL::IfOp() {
    nextState(0);
    addString("IfOp");

    if (iter->first != "kwif") return false;
    setLexem();
    nextState(1);
    addString("kwif");
    backStateIt();
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
    int tempHeight = treeHeight;
    if (!ElsePart()) {
        output.erase(output.end() - treeHeight + tempHeight, output.end());
        return false; }
    backStateIt();
    backStateIt();
    return true;
}

bool LL::ElsePart() {
    nextState(0);
    addString("ElsePart");
    if (iter->first == "kwelse"){
        nextState(0);
        addString("kwelse Stmt");
        setLexem();
        if(!Stmt()) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}
//
// block "SwitchOp"
bool LL::SwitchOp() {
    nextState(0);
    addString("SwitchOp");

    if (iter->first != "kwswitch") return false;
    setLexem();
    nextState(0);
    addString("kwswitch");
    backStateIt();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");
    if (!E()) return false;
    if (iter->first != "rpar") return false;
    setLexem();
    nextState(1);
    addString("rpar");
    backStateIt();
    if (iter->first != "lbrace") return false;
    setLexem();
    nextState(1);
    addString("lbrace Cases");

    if (!Cases()) return false;
    if (iter->first != "rbrace") return false;
    setLexem();
    nextState(0);
    addString("rbrace");
    backStateIt();
    backStateIt();
    backStateIt();
    return true;
}

bool LL::Cases() {
    nextState( 1);
    addString("Acase");
    if (!ACase()) return false;
    backStateIt();
    nextState(0);
    addString("CasesList");
    if (!CasesList()) return false;
    backStateIt();
    backStateIt();
    return true;
}

bool LL::CasesList() {
    vector<Lexem>::iterator it = iter;
    int tempHeight = treeHeight;

    nextState(1);
    addString("Acase");
    if (ACase()){
        backStateIt();
        nextState(0);
        addString("CasesList");
        if (!CasesList()) return false;
        backStateIt();
        return true;
    }
    else {
        for (int i = 0; i < treeHeight - tempHeight; i++) backStateIt();
        iter = it;
        output.pop_back();
    }
    return true;
}

bool LL::ACase() {
    if (iter->first == "kwcase"){
        setLexem();
        nextState(1);
        addString("kwcase");
        backStateIt();
        if (iter->first == "num" or iter->first == "char"){
            setLexem();
            nextState(1);
            addString(" " + iter->second);
            backStateIt();
            if (iter->first != "colon") return false;
            setLexem();
            nextState(0);
            addString("colon StmtList");
            if (!StmtList()) return false;
            return true;
        }
        else return false;
    }
    else if (iter->first == "default"){
        setLexem();
        nextState(1);
        addString("kwdefault");
        backStateIt();
        if (iter->first != "colon") return false;
        setLexem();
        nextState(0);
        addString("colon StmtList");
        if (!StmtList()) return false;
        return true;
    }
    return false;
}
//
// block "IN" and "OUT"
bool LL::IOp() {
    if (iter->first != "kwin") return false;
    setLexem();
    nextState(0);
    addString("IfOp");

    nextState(1);
    addString("kwin");
    backStateIt();
    if (iter->first != "id") return false;
    setLexem();
    nextState(1);
    addString(" " + iter->second);
    backStateIt();
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(0);
    addString("semicolon");
    backStateIt();
    backStateIt();
    backStateIt();
    return true;
}

bool LL::OOp() {
    if (iter->first != "kwout") return false;
    setLexem();
    nextState(0);
    addString("OOp");

    nextState(1);
    addString("kwout E");
    if (!E()) return false;
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(0);
    addString("semicolon");
    backStateIt();
    backStateIt();
    backStateIt();
    return true;
}