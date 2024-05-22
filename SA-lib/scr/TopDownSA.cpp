//
// Created by Admin on 01.04.2024.
//

#include <TopDownSA.h>


LL::LL(Lexer &lexer, const std::string & outPathstr, ST outputAtoms, ST outputAsm): lexer(lexer){
    outPath = outPathstr;
    outPathAtom = outputAtoms;
    outPathAsm = outputAsm;
}



void LL::solve() {
    outStr.open(outPath, std::ofstream::out | std::ofstream::trunc);
    outAtoms.open(outPathAtom, std::ofstream::out | std::ofstream::trunc);
    out.open(outPathAsm, std::ofstream::out | std::ofstream::trunc);

    setLexem();
    statesIt = states.begin();
    nextState(0);
    addString("StmtList");

    bool f = StmtList("-1") and *iter == LEX_EOF;

    cout << "Execution results:" << endl;

    if (!printTree(f)) {
        cout << "Result: fail" << endl;
        }

    else if (!printAtoms()) {
        cout << "Result: fail" << endl;
        }

    else if (!asmBlock()) {
        cout << "Result: fail" << endl;
        }
    else cout << "Result: all blocks are completed successfully" << endl;
    outStr.close();
    outAtoms.close();
    out.close();
    return;
}
//
// Other func
//print SA-Tree
bool LL::printTree(bool fl) {
    cout << "-Syntax block: ";

    if (fl) {
        for (const auto &now: output) {
            outStr << now << endl;
            //cout << now << endl;
        }
        outStr << endl << "Result: Accept" << endl;
        cout << "The tree was painting" << endl;
    }
    else {
        outStr << endl << "Result: Error" << endl;
        cout << "Syntax error" << endl;
        return false;
    }
    return true;
}
//
// print Atoms and semantic verification
bool LL::printAtoms() {

    cout << "-Semantic block: ";
    for (auto  now : atomList){
        if (now.scope == "$Error" or now.operation == "$Error" or now.second == "$Error" or now.first == "$Error" or now.third == "$Error"){
            outAtoms.clear();
            cout << "Semantic error" << endl;
            return false;
        }
        outAtoms << now.scope + ": " <<"(" << now.operation << ", " << now.first << ", " << now.second << ", " <<
                 now.third << ")" << endl;
    }

    outAtoms << endl << endl << "=========================================" << endl;
    outAtoms << "Code  :  Name  :  Kind  :  Type  :  Init  :  Len  :  Scope  :  Offset" << endl;

    for (auto now: table){
        outAtoms << now.code + " " + now.name + " " + now.kind + " " + now.type + " " + now.value + " " + now.len + " " + now.scope + " " + now.offset << endl;
    }
    cout << "Atoms was printed" << endl;
    return true;
}
//
// assembler block
bool LL::asmBlock() {
    //checking for the presence of a "main" (entry point)
    bool fl = false;
    for (auto now: table){
        if (now.name == "main") {
            fl = true;
            break;
        }
    }
    if (!fl) {
        cout << "-Assembler block: the function \'main\' is missing (Error)" << endl;
        return false;
    }

    cout << "-Assembler block: ";
    //
    // adding global vars and other initial preparations
    asmList.push_back("ORG 8000H");
    for (auto now: table){
        if (now.scope == "-1" and now.kind == "var"){
            asmList.push_back(now.name + ": DB " + now.value);
        }
    }
    asmList.push_back("ORG 0\nLXI H,0\nSPHL\nPUSH B");


    asmList.push_back("CALL main\nEND\n@MUL\n@DIV\n@PRINT\n\n");
    //
    //main block
    for (auto now: atomList){
        if (now.operation == "ADD") ADD(now); // done
        else if (now.operation == "SUB") SUB(now); // done
        else if (now.operation == "MUL") MUL(now); // done
        else if (now.operation == "MOV") MOV(now); // done
        else if (now.operation == "LBL") LBL(now); // done
        else if (now.operation == "JMP") JMP(now); // done
        else if (now.operation == "AND") AND(now); // done
        else if (now.operation == "OR") OR(now); // done
        else if (now.operation == "EQ") EQ(now); // done
        else if (now.operation == "NE") NE(now); // done
        else if (now.operation == "LT") LT(now); // done
        else if (now.operation == "LE") LE(now); // done
        else if (now.operation == "GT") GT(now); // done
        else if (now.operation == "NOT") NOT(now); // done
        else if (now.operation == "IN") IN(now); // done
        else if (now.operation == "OUT") OUT(now); // done
        else if (now.operation == "PARAM") PARAM(now);
        else if (now.operation == "CALL") CALL(now);
        else if (now.operation == "RET") RET(now); // done
    }
    //
    //printing assembly code
    printAsm();
    cout << "Assembler code has been generated successfully" << endl;
    return true;
}

void LL::printAsm() {
    for (auto now: asmList){
        out << now << endl;
    }
    return;
}

void LL::loadOp(ST operand, ST scope, int newOffset = -1) {
    if (operand[0] != '\''){
        asmList.push_back("MVI A, " + operand);
        return;
    }
    else if (operand[0] == '\"'){
        asmList.push_back("MVI A, " + operand.substr(1,operand.size() - 2));
        return;
    }
    //cout << "loadOp: " << stoi(operand) << endl;
    object item = table[stoi(operand.substr(1, operand.size() - 2)) - 1];
    if (item.scope == "-1"){
        asmList.push_back("LDA " + item.name);
    }
    else{
        if (newOffset != -1){
            asmList.push_back("LXI H, " + to_string(newOffset));

        }
        else{
            asmList.push_back("LXI H, " + item.offset);
        }
        asmList.push_back("DAD SP");
        asmList.push_back("MOV A, M");
    }
}

void LL::saveOp(ST operand) {
    //cout << operand << endl;
    object item = table[stoi(operand.substr(1, operand.size() - 2)) - 1];
    if (item.scope == "-1"){
        asmList.push_back("STA " + item.name);
    }
    else {
        asmList.push_back("LXI H, " + item.offset);
        asmList.push_back("DAD SP");
        asmList.push_back("MOV M, A");
    }
}
//
//assembly operations (functions)
void LL::ADD(const LL::atom &atom) {
    asmList.push_back("\n\t; ADD block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("ADD B");
    saveOp(atom.third);
}

void LL::SUB(const LL::atom &atom) {
    asmList.push_back("\n\t; SUB block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("SUB B");
    saveOp(atom.third);
}

void LL::MUL(const LL::atom &atom) {
    asmList.push_back("\n\t; MUL block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV D, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("MOV C, A");

    asmList.push_back("CALL @MULT");

    asmList.push_back("MOV A, B");
    saveOp(atom.third);
}

void LL::MOV(const LL::atom &atom) {
    asmList.push_back("\n\t; MOV block");
    loadOp(atom.first, atom.scope);
    saveOp(atom.third);

}

void LL::LBL(const LL::atom &atom) {
    asmList.push_back(atom.third + ":");
    if (atom.third[0] != '$'){
        for (auto now: table){
            if (now.name == atom.third){
                for (int i = 0; i < stoi(now.offset) - stoi(now.len); i++){
                    asmList.push_back("PUSH B");
                }
                break;
            }
        }
    }
}

void LL::JMP(const LL::atom &atom) {
    asmList.push_back("JMP " + atom.third);
}

void LL::AND(const LL::atom &atom) {
    asmList.push_back("\n\t; AND block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("ANA B");
    saveOp(atom.third);
}

void LL::OR(const LL::atom &atom) {
    asmList.push_back("\n\t; OR block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("ORA B");
    saveOp(atom.third);
}

void LL::EQ(const LL::atom &atom) {
    asmList.push_back("\n\t; EQ block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMP B");
    asmList.push_back("JZ " + atom.third);
}

void LL::NE(const LL::atom &atom) {
    asmList.push_back("\n\t; NE block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMP B");
    asmList.push_back("JNZ " + atom.third);
}

void LL::LT(const LL::atom &atom) {
    asmList.push_back("\n\t; LT block");
    loadOp(atom.second,atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMP B");
    asmList.push_back("JM " + atom.third);
}

void LL::LE(const LL::atom &atom) {
    asmList.push_back("\n\t; LE block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMP B");
    asmList.push_back("JZ " + atom.third);
    asmList.push_back("JM" + atom.third);
}

void LL::GT(const LL::atom &atom) {
    asmList.push_back("\n\t; GT block");
    loadOp(atom.second, atom.scope);
    asmList.push_back("MOV B, A");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMP B");
    asmList.push_back("JP " + atom.third);
}

void LL::NOT(const LL::atom &atom) {
    asmList.push_back("\n\t; NOT block");
    loadOp(atom.first, atom.scope);
    asmList.push_back("CMA");
    saveOp(atom.third);
}

void LL::IN(const LL::atom &atom) {
    asmList.push_back("\n\t; IN block");
    asmList.push_back("IN 0");
    saveOp(atom.third);
}

void LL::OUT(const LL::atom &atom) {
    asmList.push_back("\n\t; OUT block");
    loadOp(atom.third, atom.scope);
    asmList.push_back("OUT 1");
}

void LL::PARAM(const LL::atom &atom) {
    programStack.push(atom.third);
}

void LL::CALL(const LL::atom &atom) {
    asmList.push_back("\n\t; CALL block");
    asmList.push_back("PUSH B");

    object func = table[stoi(atom.first.substr(1, atom.first.size() - 2)) - 1];
    int n = stoi(func.len);
    int total = stoi(func.offset);
    string funcCode = func.code.substr(1, func.code.size() - 2);

    for (int i = 0; i < n; i++){
        asmList.push_back("PUSH B");
    }
    for (int i = n - 1; i >= 0; i--){
        string item = programStack.top();
        //cout << atom.scope << endl;
        programStack.pop();
        int newOffset = stoi(table[stoi(funcCode) + i].offset) - (i + 1 != n ? stoi(table[stoi(funcCode) + i + 1].offset): 0);
        loadOp(item, atom.scope, newOffset);


        asmList.push_back("LXI H, " + to_string((i + 1 ) * 2));
        asmList.push_back("DAD SP");
        asmList.push_back("MOV M, A");
    }
    asmList.push_back("CALL " + func.name);

    for (int i = 0; i < n; i++){
        asmList.push_back("POP B");
    }
    asmList.push_back("POP B");
    asmList.push_back("MOV A, C");
    saveOp(atom.third);
}

void LL::RET(const LL::atom &atom) {
    int m = stoi(table[stoi(atom.scope) - 1].offset);
    asmList.push_back("\n\t; RET block");
    loadOp(atom.third, atom.scope);
    asmList.push_back("LXI H, " + to_string(m*2 + 2));
    asmList.push_back("DAD SP");
    asmList.push_back("MOV M, A");
    for (int i = 0; i < m; i++){
        asmList.push_back("POP");
    }
    asmList.push_back("RET");
}
//

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
    string label = to_string(labelCounter);
    labelCounter++;
    return label;
}

string LL::alloc(const string &scope) {
    tempVarCounter++;
    return addVar("$T" + to_string(tempVarCounter), scope, "kwint");
}

string LL::addVar(const std::string &name, const string &scope, const std::string type, const std::string &init) {
    bool fl = false;
    for (int i = 0; i < table.size(); i++) {
        if (table[i].name == name and table[i].scope == scope){

            if (table[i].kind != "var" or table[i].type != type){
                fl = true;
                break;
            }
            table[i].value = init;
            return table[i].code;
        }
    }
    if (fl) return "$Error";
    codeCounter++;
    table.push_back({"'" + to_string(codeCounter) + "'", name, "var", type, "-1", init, scope});
    return "'" + to_string(codeCounter) + "'";
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
    table.push_back({"'" + to_string(codeCounter) + "'", name, "func", type});
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
// Atom func
void LL::addAtom(LL::atom data) {
    if (atomList.size() == 1 and atomList[0].scope == "$Error") return;
    if (data.scope == "$Error" or data.operation == "$Error" or data.first == "$Error" or data.second == "$Error" or data.third == "$Error"){
        atomList.clear();
        atomList.push_back({"$Error", "$Error", "$Error", "$Error", "$Error"});
    }
    atomList.push_back(data);
    return;
}

//
//Expression Grammer
PBS LL::E(ST scope) {
    nextState(0);
    addString("E7");
    auto result = E7(scope);
    if (!result.first) {return {false, ""}; }
    backStateIt();
    return {true, result.second};
}

PBS LL::E7(ST scope){

    nextState(1);
    addString("E6");
    auto result = E6(scope);
    if (!result.first) {return {false, ""}; }

    nextState(0);
    addString("E7\'");
    result = E7list(scope, result.second);
    if (!result.first) {return {false, ""}; }
    backStateIt();
    return {true, result.second};
}

PBS LL::E7list(ST scope, ST p){
    if (iter->first == "opor"){
        setLexem();

        nextState(1);
        addString("opor E6");

        auto result = E6(scope);
        if (!result.first) return {false, ""};
        nextState(0);
        addString("E7\'");

        auto s = alloc(scope);
        addAtom({scope, "OR", p, result.second, s});

        result = E7list(scope, s);
        if (!result.first) {return {false, ""}; }
        backStateIt();
        return {true, result.second};
    }
    backStateIt();
    return {true, p};
}

PBS LL::E6(ST scope){
    nextState(1);
    addString("E5");

    auto result = E5(scope);
    if (!result.first) {return {false, ""}; }

    nextState(0);
    addString("E6\'");

    result = E6list(scope, result.second);
    if (!result.first) {return {false, ""};}
    backStateIt();
    return {true, result.second};
}

PBS LL::E6list(ST scope, ST p){
    if (iter->first == "opand"){
        setLexem();

        nextState(1);
        addString("opand E5");

        auto result = E5(scope);
        if (!result.first) {return {false, ""}; }

        nextState(0);
        addString("E6\'");

        auto s = alloc(scope);
        addAtom({scope, "AND", p, result.second, s});

        result = E6list( scope, s);
        if (!result.first) {return {false, ""}; }
        backStateIt();
        return {true, result.second};
    }
    backStateIt();
    return {true, p};
}

PBS LL::E5(ST scope){

    nextState(1);
    addString("E4");

    auto result = E4(scope);
    if (!result.first) {return {false, ""}; }

    nextState(0);
    addString("E5\'");

    result = E5list(scope, result.second);
    if (!result.first) {return {false, ""}; }
    backStateIt();
    return {true, result.second};
}

PBS LL::E5list(ST scope, ST p){
    if (iter->first == "opeq" or iter->first == "opne" or iter->first == "oplt" or iter->first == "opgt" or
            iter->first == "ople"){
        auto item = iter->first.substr(2, iter->first.size());
        transform(item.begin(), item.end(), item.begin(), ::toupper);
        nextState(0);
        //cout << item <<endl;
        addString(iter->first + " E4");
        setLexem();

        auto result = E4(scope);
        if (!result.first) {return {false, ""}; }

        auto l = newLabel();
        auto s = alloc(scope);
        addAtom({scope, "MOV", "1", "", s});
        addAtom({scope, item, p, result.second, "$L" + l});
        addAtom({scope, "MOV", "0", "", s});
        addAtom({scope, "LBL", "", "", "$L" + l});

        backStateIt();
        return {true, s};
    }
    backStateIt();
    return {true, p};
}

PBS LL::E4(ST scope){

    nextState(1);
    addString("E3");

    auto result = E3(scope);
    if (!result.first) {return {false, ""}; }

    nextState(0);
    addString("E4\'");

    result = E4list(scope, result.second);
    if (!result.first) {return {false, ""}; }
    backStateIt();
    return {true, result.second};
}

PBS LL::E4list(ST scope, ST p){
    if (iter->first == "opplus" or iter->first == "opminus"){
        auto item = iter->first == "opplus" ? "ADD": "SUB";
        nextState(1);
        addString(iter->first + " E3");
        setLexem();

        auto result = E3(scope);
        if (!result.first) {return {false, ""}; }

        nextState(0);
        addString("E4\'");

        auto s = alloc(scope);
        addAtom({scope, item, p, result.second, s});

        result = E4list(scope, s);
        if (!result.first) {return {false, ""}; }

        backStateIt();
        return {true, result.second};
    }
    backStateIt();
    return {true, p};
}

PBS LL::E3(ST scope){

    nextState(1);
    addString("E2");

    auto result = E2(scope);
    if (!result.first) {return {false, ""}; }

    nextState(0);
    addString("E3\'");

    result = E3list(scope, result.second);
    if (!result.first) {return {false, ""}; }
    backStateIt();
    return {true, result.second};
}

PBS LL::E3list(ST scope, ST p){
    if (iter->first == "opmul"){
        setLexem();

        nextState(1);
        addString("opmul E2");

        auto result = E2(scope);
        if (!result.first) {return {false, ""}; }

        nextState(0);
        addString("E3\'");

        auto s = alloc(scope);
        addAtom({scope, "MUL", p, result.second, s});

        result = E3list(scope, s);
        if (!result.first) {return {false, ""}; }

        backStateIt();
        return {true, result.second};
    }
    backStateIt();
    return {true, p};
}

PBS LL::E2(ST scope){
    if (iter->first == "opnot"){
        setLexem();
        nextState(0);
        addString("opnot E1");

        auto result = E1(scope);
        if (!result.first){return {false, ""};}
        backStateIt();

        auto r = alloc(scope);
        addAtom({scope, "NOT", result.second, "", r});
        return {true, r};
    }
    else {
        nextState(0);
        addString("E1");

        auto result = E1(scope);
        if (!result.first) { return {false, ""}; }

        backStateIt();
        return {true, result.second};
    }
}

PBS LL::E1(ST scope){
    if (iter->first == "opinc"){
        setLexem();

        if (iter->first != "id") {return {false, ""}; }

        auto q = checkVar(scope, iter->second);
        addAtom({scope, "ADD", q, "1", q});

        nextState(0);
        addString("opinc " + iter->second);
        backStateIt();
        backStateIt();

        setLexem();
        return {true, q};
    }
    else if (iter->first == "num" or iter->first == "char"){
        auto item = iter->second;

        nextState(0);
        addString(iter->second);
        setLexem();
        backStateIt();
        backStateIt();

        return {true, item };
    }
    else if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar E");

        auto result = E(scope);
        if (!result.first) {return {false, ""}; }

        nextState(0);
        addString("rpar");
        if (iter->first != "rpar") {return {false, ""}; }
        backStateIt();
        backStateIt();
        setLexem();
        return {true, result.second};
    }
    else if (iter->first == "id"){
        auto name = iter->second;

        nextState(0);
        addString(name + " E1\'");
        setLexem();

        auto result = E1List(scope, name);
       // cout << name <<" qq: " << result.second << endl;

        if (!result.first) {return {false, ""}; }
        backStateIt();
        return {true, result.second};
    }

    return {false, ""};
}

PBS LL::E1List(ST scope, ST p) {
    if (iter->first == "lpar"){
        setLexem();

        nextState(1);
        addString("lpar Arglist");

        auto result = Arglist(scope);
        if (!result.first) {return {false, ""}; }

        nextState(0);
        addString("rpar");
        if (iter->first != "rpar") {return {false, ""}; }
        setLexem();
        backStateIt();
        backStateIt();

        auto s = checkFunc(p, result.second);
        auto r = alloc(scope);
        addAtom({scope, "CALL", s, "", r});

        return {true, r};
    }
    else if (iter->first == "opinc") {
        setLexem();

        nextState(0);
        addString("opinc");
        backStateIt();
        backStateIt();

        auto s = checkVar(scope, p);
        auto r = alloc(scope);
        addAtom({scope, "MOV", s, "", r});
        addAtom({scope, "ADD", s, "1", s});

        return {true, r};
    }
    backStateIt();
    auto q = checkVar(scope, p);
    //cout << "qq: " << q << endl;
    return {true, q};
}

//Entry point
bool LL::StmtList(ST scope) {
    if (*iter == LEX_EOF) return true;

    nextState(1);
    addString("Stmt");
    auto listIt = iter;
    if (!Stmt(scope)){
        iter = listIt;
        backStateIt();
        backStateIt();
        output.pop_back();
        return true;
    }
    nextState(0);
    addString("StmtList");
    if (!StmtList(scope)) return false;
    backStateIt();
    return true;
}

bool LL::Stmt(ST scope) {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "kwint" or iter->first == "kwchar"){
        nextState(0);
        addString("DeclareStmt");

        if (!DeclareStmt(scope)) return false;
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

    if (scope == "-1") return false;

    if (iter->first == "id"){
        nextState(0);
        addString("AssignOrCallOp");

        if (!AssignOrCallOp(scope)) return false;

        backStateIt();
        return true;
    }

    if (iter->first == "kwwhile"){
        nextState(0);
        addString("kwwhile WhileOp");

        if (!WhileOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwfor"){
        nextState(0);
        addString("kwfor ForOp");
        if (!ForOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwif"){
        nextState(0);
        addString("kwif IfOp");
        if (!IfOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwswitch"){
        nextState(0);
        addString("kwswitch SwitchOp");
        if (!SwitchOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwin"){
        nextState(0);
        addString("kwin IOp");
        if (!IOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "kwout"){
        nextState(0);
        addString("kwout OOp");
        if (!OOp(scope)) return false;
        backStateIt();
        return true;
    }

    if (iter->first == "lbrace"){
        setLexem();
        nextState(1);
        addString("lbrace StmtList");
        if (!StmtList(scope)) {return false; }
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

        auto result = E(scope);
        if (!result.first) {return false; }
        if (iter->first != "semicolon") {return false; }

        nextState(0);
        addString("semicolon");
        setLexem();
        backStateIt();
        backStateIt();

        addAtom({scope, "RET", "", "", result.second});
        return true;
    }

    return false;
}
//
//Declare block
PBS LL::Type(ST scope) {
    if (*iter == LEX_EOF) return {false, ""};

    if (iter->first == "kwint" or iter->first == "kwchar"){
        auto item = iter->first;

        nextState(0);
        addString(iter->first);
        setLexem();
        backStateIt();
        backStateIt();
        return {true, item};
    }
    return {false, ""};
}

bool LL::DeclareStmt(ST scope) {
    if (*iter == LEX_EOF) return false;

    nextState(1);
    addString("Type");

    auto result = Type(scope);
    if (!result.first) {return false; }
    if (iter->first != "id") return false;
    auto item = iter->second;

    nextState(0);
    addString(" " + item + " DeclareStmt'");
    setLexem();
    if (!DeclareStmtList(scope, result.second, item)) return false;
    backStateIt();
    return true;

}

bool LL::DeclareStmtList(ST scope, ST p, ST q) { // p = type, q = name
    if (*iter == LEX_EOF) return false;
    if (iter->first == "lpar"){
        if (scope != "-1") return false;
        setLexem();
        nextState(1);
        addString("lpar ParamList");

        string codeFunc = addFunc(q, p);

        auto result = ParamList(codeFunc);
        if (!result.first) return false;


        table[stoi(codeFunc) - 1].len = result.second;

        if (iter->first != "rpar") return false;
        setLexem();
        if (iter->first != "lbrace") return false;
        setLexem();
        nextState(1);
        addString("rpar lbrace StmtList");

        addAtom({codeFunc, "LBL", "", "", q});
        if (!StmtList(codeFunc)) return false;
        if (iter->first != "rbrace") return false;
        setLexem();
        nextState(0);
        addString("rbrace");
        backStateIt();
        backStateIt();
//
//offset block control
        int counter = 0;
        int total_cnt = 0;
        for (int i = table.size() - 1; i > stoi(codeFunc) - 1 + stoi(result.second); i--){
            if (table[i].scope == codeFunc){
                table[i].offset = to_string(counter);
                counter += 2;
                total_cnt++;
            }
        }

        counter += 2;

        for (int i = stoi(codeFunc) + stoi(result.second) - 1; i > stoi(codeFunc) - 1; i-- ){
            table[i].offset = to_string(counter);
            counter += 2;
            total_cnt++;
        }
        table[stoi(codeFunc) - 1].offset = to_string(total_cnt); // the number of vars in the function
//

        addAtom({codeFunc, "RET", "", "", "0"});
        return true;
    }
    else if (iter->first == "opassign"){
        setLexem();
        auto item = iter->second;

        if (iter->first == "num"){
            string var = addVar(q, scope, p, item);

            if (var == "'$Error'") return false;

            nextState(1);
            addString("opassign " + item + " DeclareVarList");
            setLexem();

            addAtom({scope, "MOV", item, "", var});

            if(!DeclareVarList(scope, p)) return false;
            if (iter->first != "semicolon") return false;
            setLexem();
            nextState(0);
            addString("semicolon");
            backStateIt();
            backStateIt();
            return true;
        }

        else if (iter->first == "char"){
            string var = addVar(q, scope, p, item);
            if (var == "'$Error'") return false;

            nextState(1);
            addString("opassign " + item + " DeclareVarList");
            setLexem();
            addAtom({scope, "MOV", item, "", var});
            if(!DeclareVarList(scope, p)) return false;
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
        string var = addVar(q, scope, p);
        if (var == "$Error")

        nextState(1);
        addString("DeclareVarList");
        if (!DeclareVarList(scope, p)) return false;
        if (iter->first != "semicolon") return false;
        setLexem();
        nextState(0);
        addString("semicolon");
        backStateIt();
        backStateIt();
        return true;
    }
}

bool LL::DeclareVarList(ST scope, ST p) {
    if (*iter == LEX_EOF) return false;
    //cout << "!!" << endl;

    if (iter->first == "comma") {
        setLexem();

        if (iter->first != "id") return false;
        auto item = iter->second;

        nextState(1);
        addString("comma " + item + " InitVar");
        setLexem();
        if (!InitVar(scope, p, item)) return false;
        nextState(0);
        addString("DeclareVarList");
        if (!DeclareVarList(scope, p)) return false;
        backStateIt();
        return true;
    }
    backStateIt();
    return true;
}

bool LL::InitVar(ST scope, ST p, ST q) {
    if (*iter == LEX_EOF) return false;

    if (iter->first == "opassign"){
        setLexem();

        if (iter->first != "num" and iter->first != "char") return false;
        string var = addVar(q, scope, p, iter->second);
        if (var == "'$Error'") return false;

        addAtom({scope, "MOV", iter->second, "", var});

        nextState(0);
        addString("opassign " + iter->second);
        setLexem();
        backStateIt();
        backStateIt();
        return true;
    }
    string var = addVar(q, scope, p);
    if (var == "'$Error'") return false;

    backStateIt();
    return true;
}

PBS LL::ParamList(ST scope) {
    if (*iter == LEX_EOF) return {false, ""};

    if (iter->first == "kwint" or iter->first == "kwchar"){
        nextState(1);
        addString("Type");
        auto type = Type(scope);
        if (iter->first != "id") return {false, ""};
        string var = addVar(iter->second, scope, type.second);
        if (var == "'$Error'") return {false, ""};

        nextState(0);
        addString(" " + iter->second + " ParamList'");
        setLexem();

        auto result = ParamListList(scope);
        if (!result.first) return {false, ""};
        backStateIt();
        return {true, to_string(stoi(result.second) + 1)};
    }
    backStateIt();
    return {true, "0"};
}

PBS LL::ParamListList(ST scope) {
    if (*iter == LEX_EOF) return {false, ""};

    if (iter->first == "comma"){
        setLexem();
        if (iter->first != "kwint" and iter->first != "kwchar") {
            backStateIt();
            return {true, "0"}; }
        nextState(1);
        addString("comma Type");

        auto type = Type(scope);
        if (iter->first != "id") return {false, ""};
        string var = addVar(iter->second, scope, type.second);
        if (var == "'Error'") return {false, ""};

        nextState(0);
        addString(" " + iter->second + " ParamList'");
        setLexem();

        auto result = ParamListList(scope);
        if (!result.first) return {false, ""};
        backStateIt();
        return {true, to_string(stoi(result.second) + 1)};
    }
    backStateIt();
    return {true, "0"};
}
//
// ArgList
PBS LL::Arglist(ST scope) {
    //if (iter == list.end()) cout << "!!!!!!!!!!!!!!!!" << endl;
    if (iter->first != "num" and iter->first != "char" and iter->first != "id" and iter->first != "opinc" and iter->first != "lpar"){
        backStateIt();
        return {true, "0"};
    }
    nextState(1);
    addString(iter->second + " E");

    auto result1 = E(scope);
    if (!result1.first) return {false, ""};
    nextState(0);
    addString("ArgList'");

    addAtom({scope, "PARAM", "", "", result1.second});

    auto result = ArgListList(scope);
    if(!result.first) return {false, ""};
    backStateIt();


    return {true, to_string(stoi(result.second) + 1)};
}

PBS LL::ArgListList(ST scope) {
    if (iter->first == "comma"){
        setLexem();
        if (iter->first != "num" and iter->first != "char" and iter->first != "id" and iter->first != "opinc" and iter->first != "lpar")
            return {false, ""};
        nextState(1);
        addString("comma E");

        auto result1 = E(scope);
        if (!result1.first) { return {false, ""};}

        nextState(0);
        addString("Arglist'");

        auto result = ArgListList(scope);
        if (!result.first) return {false, ""};

        backStateIt();

        addAtom({scope, "PARAM", "", "", result1.second});
        return {true, to_string(stoi(result.second) + 1)};
    }
    backStateIt();
    return {true, "0"};
}
//
// Assign Or Call operation block
bool LL::AssignOrCallOp(ST scope) {
    nextState(1);
    addString("AssignOrCall");
    if (!AssignOrCall(scope)) return false;
    if (iter->first != "semicolon") return false;
    nextState(0);
    addString("semicolon");
    setLexem();
    backStateIt();
    backStateIt();
    return true;
}

bool LL::AssignOrCall(ST scope) {
    if (iter->first != "id") return false;
    auto item = iter->second;

    nextState(0);
    addString(item + " AssignOrCall'");
    setLexem();
    if (!AssignOrCallList(scope, item)) return false;
    backStateIt();
    return true;
}

bool LL::AssignOrCallList(ST scope, ST p) {
    if (iter->first == "opassign"){
        setLexem();
        nextState(0);
        addString("opassign E");

        auto result = E(scope);
        if (!result.first) return false;
        backStateIt();

        auto r = checkVar(scope, p);
        addAtom({scope, "MOV", result.second, "", r});

        return true;
    }
    else if (iter->first == "lpar"){
        setLexem();
        nextState(1);
        addString("lpar Arglist");

        auto result = Arglist(scope);
        if (!result.first) return false;
        if (iter->first != "rpar") return false;
        nextState(0);
        addString("rpar");
        setLexem();
        backStateIt();
        backStateIt();

        auto q = checkFunc(p, result.second);
        auto r = alloc(scope);
        addAtom({scope, "CALL", q, "", r});

        return true;
    }
    return false;
}//
//while block
bool LL::WhileOp(ST scope) {
    setLexem();

    auto label1 = newLabel();
    auto label2 = newLabel();
    addAtom({scope, "LBL", "", "", "$L" + label1});

    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");

    auto result = E(scope);
    if (!result.first) return false;

    addAtom({scope, "EQ", result.second, "0", "$L" + label2});

    if (iter->first != "rpar") return false;
    setLexem();
    nextState(0);
    addString("rpar Stmt");
    if (!Stmt(scope)) return false;
    backStateIt();

    addAtom({scope, "JMP", "", "", "$L" + label1});
    addAtom({scope, "LBL", "", "", "$L" + label2});

    return true;
}
//
// block "ForOp"
bool LL::ForOp(ST scope) {
    setLexem();

    auto label1 = newLabel();
    auto label2 = newLabel();
    auto label3 = newLabel();
    auto label4 = newLabel();

    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar ForInit");
    if (!ForInit(scope)) return false;
    if (iter->first != "semicolon") return false;

    setLexem();
    backStateIt();
    nextState(1);
    addString("semicolon ForExp");

    addAtom({scope, "LBL", "", "", "$L" + label1});

    auto result = ForExp(scope);
    if (!result.first) return false;

    addAtom({scope, "EQ", result.second, "0", "$L" + label4});
    addAtom({scope, "JMP", "", "", "$L" + label3});
    addAtom({scope, "LBL", "", "", "$L" + label2});

    nextState(1);
    addString("semicolon ForLoop");
    if (!ForLoop(scope)) return false;

    addAtom({scope, "JMP", "", "", "$L" + label1});
    addAtom({scope, "LBL", "", "", "$L" + label3});

    nextState(0);
    addString("rpar Stmt");
    if (!Stmt(scope)) {return false; }

    addAtom({scope, "JMP", "", "", "$L" + label2});
    addAtom({scope, "LBL", "", "", "$L" + label4});

    backStateIt();
    return true;
}

bool LL::ForInit(ST scope) {
    if (iter->first == "id"){
        nextState(0);
        addString("AssignOrCall");
        if (!AssignOrCall(scope)) return false;
    }
    return true;
}

PBS LL::ForExp(ST scope) {
    bool fl = false;
    PBS result;
    if (iter->first == "lpar" or iter->first == "id" or iter->first == "num" or iter->first == "char" or iter->first == "opinc"){
        nextState(0);
        addString("E");

        result = E(scope);
        fl = !result.first? true: false;
    }
    if (iter->first == "semicolon") {
        setLexem();
        backStateIt();

        if (fl) return {true, "1"};
        return {true, result.second};
    }

    return {false, ""};
}

bool LL::ForLoop(ST scope) {
    if (iter->first == "opinc"){
        setLexem();

        if (iter->first != "id") return false;
        auto item = iter->second;
        setLexem();

        auto p = checkVar(scope, item);
        addAtom({scope, "ADD", p, "1", p});

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
        if (!AssignOrCall(scope)) return false;
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
bool LL::IfOp(ST scope) {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");

    auto result = E(scope);
    if (!result.first) return false;
    if (iter->first != "rpar") return false;
    setLexem();

    auto label1 = newLabel();
    addAtom({scope, "EQ", result.second, "0", "$L" + label1});

    nextState(1);
    addString("rpar Stmt");
    if (!Stmt(scope)) return false;

    auto label2 = newLabel();
    addAtom({scope, "JMP", "", "", "$L" + label2});
    addAtom({scope, "LBL", "", "", "$L" + label1});

    nextState(0);
    addString("ElsePart");
    if (iter->first == "kwelse"){
        if (!ElsePart(scope)) return false;

        addAtom({scope, "LBL", "", "", "$L" + label2});

        backStateIt();
        return true;
    }
    addAtom({scope, "LBL", "", "", "$L" + label2});

    backStateIt();
    backStateIt();
    return true;
}

bool LL::ElsePart(ST scope) {
    nextState(0);
    addString("kwelse Stmt");
    setLexem();
    if(!Stmt(scope)) return false;
    backStateIt();
    return true;
}
//
// block "SwitchOp"
bool LL::SwitchOp(ST scope) {
    setLexem();
    if (iter->first != "lpar") return false;
    setLexem();
    nextState(1);
    addString("lpar E");

    auto result = E(scope);
    //cout << "qq: " << result.second << endl;
    if (!result.first) return false;
    if (iter->first != "rpar") return false;
    setLexem();

    if (iter->first != "lbrace") return false;
    setLexem();
    nextState(1);
    addString("lpar lbrace Cases");

    auto end = newLabel();
    if (!Cases(scope, result.second, end)) return false;
    //cout << 1 << endl;
    if (iter->first != "rbrace") return false;
    setLexem();
    nextState(0);
    addString("rbrace");
    backStateIt();
    backStateIt();

    addAtom({scope, "LBL", "", "", "$L" + end});
    return true;
}

bool LL::Cases(ST scope, ST p, ST end) {
    nextState( 1);
    addString("Acase");

    auto result = ACase(scope, p, end);
    if (!result.first) return false;
    //cout << 2 << endl;
    nextState(0);
    addString("CasesList");
    if (!CasesList(scope, p, end, result.second)) return false;
    backStateIt();
    return true;
}

bool LL::CasesList(ST scope, ST p, ST end, ST def) {
    if (iter->first == "kwdefault" or iter->first == "kwcase") {
        nextState(1);
        addString("Acase");


        auto result = ACase(scope, p, end);
        if (!result.first) return false;

        if (stoi(def) >= 0 and stoi(result.second) >= 0){
            addAtom({"$Error", "$Error", "$Error", "$Error", "$Error"});
        }
        auto def2 = stoi(def) < stoi(result.second) ? result.second: def;

        nextState(0);
        addString("CasesList");

        if (!CasesList(scope, p, end, def2)) return false;

    }
    cout << def << " " << end << endl;
    string q = stoi(def) >= 0 ? def: end;
    addAtom({scope, "JMP", "", "", "$L" + q});

    backStateIt();
    return true;
}

PBS LL::ACase(ST scope, ST p, ST end) {
    if (iter->first == "kwcase"){
        setLexem();
        if (iter->first != "num" and iter->first != "char")return {false, ""};
        auto item = iter->second;
        //cout << p  << "qq"<< endl;
        setLexem();

        string next = newLabel();
        addAtom({scope, "NE", p, item, "$L" + next});

        if (iter->first != "colon") return {false, ""};
        nextState(0);
        addString("kwcase " + item + " colon StmtList");
        setLexem();
        if (!StmtList(scope)) return {false, ""};

        addAtom({scope, "JMP", "", "", "$L" + end});
        addAtom({scope, "LBL", "", "", "$L" + next});

        backStateIt();
        return {true, "-1"};

    }
    else if (iter->first == "kwdefault"){
        setLexem();
        //cout << "qq" << endl;

        if (iter->first != "colon") return {false, ""};
        setLexem();
        nextState(0);
        addString("kwdefault colon StmtList");

        string next = newLabel();
        string def = newLabel();

        addAtom({scope, "JMP", "", "", "$L" + next});
        addAtom({scope, "LBL", "", "", "$L" + def});

        if (!StmtList(scope)) return {false, ""};
        backStateIt();

        addAtom({scope, "JMP", "", "", "$L" + end});
        addAtom({scope, "LBL", "", "", "$L" + next});

        return {true, def};
    }
    return {false, ""};
}
//
// block "IN" and "OUT"
bool LL::IOp(ST scope) {
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

    auto p = checkVar(scope, item);
    addAtom({scope, "IN", "", "", p});

    return true;
}

bool LL::OOp(ST scope) {
    setLexem();

    nextState(1);
    addString("OOp'");
    if (!OOpList(scope)) return false;
    if (iter->first != "semicolon") return false;
    setLexem();
    nextState(0);
    addString("semicolon");
    backStateIt();
    backStateIt();
    return true;
}

bool LL::OOpList(ST scope) {
    if (iter->first == "str"){
        auto item = iter->second;
        nextState(0);
        addString("\"" + item + "\"");
        setLexem();
        backStateIt();
        backStateIt();

        addAtom({scope, "OUT", "", "", "\"" + item + "\""});
        return true;
    }
    nextState(0);
    addString("E");
    auto result = E(scope);
    if (!result.first) return false;
    backStateIt();

    addAtom({scope, "OUT", "", "", result.second});
    return true;
}