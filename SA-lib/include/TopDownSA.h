//
// Created by Admin on 01.04.2024.
//

#ifndef LEX_TESTS_TOPDOWNSA_H
#define LEX_TESTS_TOPDOWNSA_H

#endif //LEX_TESTS_TOPDOWNSA_H

#include <Lexer.h>
#include <stack>
#include <iostream>

using namespace std;
using PBS = pair<bool, string>;

class LL {
private:

    // Expression Grammer
    bool E();

    bool E7();

    bool E7list();

    bool E6();

    bool E6list();

    bool E5();

    bool E5list();

    bool E4();

    bool E4list();

    bool E3();

    bool E3list();

    bool E2();

    bool E1();

    bool E1List();
    //
    //Entry point
    bool StmtList();

    bool Stmt();
    //
    //Declare block
    bool Type();

    bool DeclareStmt();

    bool DeclareStmtList();

    bool DeclareVarList();

    bool InitVar();

    bool ParamList();

    bool ParamListList();
    //
    // Assign or Call operation block
    bool AssignOrCallOp();

    bool AssignOrCall();

    bool AssignOrCallList();
    //
    // while block
    bool WhileOp();
    //
    // block "ForOp"
    bool ForOp();

    bool ForInit();

    bool ForExp();

    bool ForLoop();
    //
    // block "IfOp"
    bool IfOp();

    bool ElsePart();
    //
    // block "SwitchOp"
    bool SwitchOp();

    bool Cases();

    bool CasesList();

    bool ACase();
    //
    // block "IN" and "OUT"
    bool IOp();

    bool OOp();

    bool OOpList();
    //
    // ArgList
    bool Arglist();

    bool ArgListList();
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
        string type = "None";
        string len = "-1";
        string value = "None";
        string scope = "-1";
    };

    vector<object> table;
    int labelCounter;
    int codeCounter;
    int tempVarCounter;
    //
    //atom block
    struct atom{
        string operation;
        string first;
        string second;
        string third;
    };
    vector<atom> atomList;
    vector<string> outputAtomList;
    //

public:
    void solve ();

    explicit LL(Lexer& lexer, const string&);
};