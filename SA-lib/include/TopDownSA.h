//
// Created by Admin on 01.04.2024.
//

#ifndef LEX_TESTS_TOPDOWNSA_H
#define LEX_TESTS_TOPDOWNSA_H

#endif //LEX_TESTS_TOPDOWNSA_H

#include <Lexer.h>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;
using PBS = pair<bool, string>;
using ST = const string&;

class LL {
private:

    // Expression Grammer
    PBS E(ST scope);

    PBS E7(ST scope);

    PBS E7list(ST scope, ST p);

    PBS E6(ST scope);

    PBS E6list(ST scope, ST p);

    PBS E5(ST scope);

    PBS E5list(ST scope, ST p);

    PBS E4(ST scope);

    PBS E4list(ST scope, ST p);

    PBS E3(ST scope);

    PBS E3list(ST scope, ST p);

    PBS E2(ST scope);

    PBS E1(ST scope);

    PBS E1List(ST scope, ST p);
    //
    //Entry point
    bool StmtList(ST scope);

    bool Stmt(ST scope);
    //
    //Declare block
    PBS Type(ST scope);

    bool DeclareStmt(ST scope);

    bool DeclareStmtList(ST scope, ST p, ST q);

    bool DeclareVarList(ST scope, ST p);

    bool InitVar(ST scope, ST p, ST q);

    PBS ParamList(ST scope);

    PBS ParamListList(ST scope);
    //
    // Assign or Call operation block
    bool AssignOrCallOp(ST scope);

    bool AssignOrCall(ST scope);

    bool AssignOrCallList(ST scope, ST p);
    //
    // while block
    bool WhileOp(ST scope);
    //
    // block "ForOp"
    bool ForOp(ST scope);

    bool ForInit(ST scope);

    PBS ForExp(ST scope);

    bool ForLoop(ST scope);
    //
    // block "IfOp"
    bool IfOp(ST scope);

    bool ElsePart(ST scope);
    //
    // block "SwitchOp"
    bool SwitchOp(ST scope);

    bool Cases(ST scope, ST p, ST end);

    bool CasesList(ST scope, ST p, ST end, ST def);

    PBS ACase(ST scope, ST p, ST end);
    //
    // block "IN" and "OUT"
    bool IOp(ST scope);

    bool OOp(ST scope);

    bool OOpList(ST scope);
    //
    // ArgList
    PBS Arglist(ST scope);

    PBS ArgListList(ST scope);
    //
    // Other funcs
    void setLexem();
    void nextState(const int& state);
    void backStateIt();
    void addString(const string& str);

    Lexer lexer;
    Lexem tempLexem;

    vector <Lexem> list;
    vector<Lexem>::iterator iter;

    // For print graph
    int treeHeight = 0;
    vector<int> states;
    vector<int>::iterator statesIt;

    ofstream outStr;
    string outPath;

    vector<string> output;

    bool printTree(bool fl);
    //
    //semantic block
    string newLabel();
    string alloc(const string & scope);
    string addVar(const string& name, const string& scope, const string type, const string& init = "0");
    string addFunc(const string& name, const string& type);
    string checkVar(const string& scope, const string& name);
    string checkFunc(const string& name, const string& len);

    struct object{
        string code;
        string name;
        string kind;
        string type = "kwint";
        string len = "-1";
        string value = "00";
        string scope = "-1";
        string offset = "-1";
    };

    vector<object> table;
    int labelCounter = 0;
    int codeCounter = 0;
    int tempVarCounter = 0;

    bool printAtoms();
    //
    //atom block
    struct atom{
        string scope = "-1";
        string operation = "";
        string first = "";
        string second = "";
        string third = "";
    };

    void addAtom(atom data);
    vector<atom> atomList;
    string outPathAtom;
    //
    //assembler block (for web asm)
    vector<string> asmList;
    string outPathAsm;
    stack<string> programStack;

    bool asmBlock();
    void printAsm();

    void loadOp(ST operand, ST scope, int newOffset);
    void saveOp(ST operand);

    //assembly commands
    void ADD(const atom& atom);

    void SUB(const atom& atom);

    void MUL(const atom& atom);

    void MOV(const atom& atom);

    void LBL(const atom& atom);

    void JMP(const atom& atom);

    void AND(const atom& atom);

    void OR(const atom& atom);

    void EQ(const atom& atom);

    void NE(const atom& atom);

    void LT(const atom& atom);

    void LE(const atom& atom);

    void GT(const atom& atom);

    void NOT(const atom& atom);

    void IN(const atom& atom);

    void OUT(const atom& atom);

    void PARAM(const atom& atom);

    void CALL(const atom& atom);

    void RET(const atom& atom);
    //

    fstream out;

public:
    void solve ();

    explicit LL(Lexer& lexer, const string&, ST, ST);
};