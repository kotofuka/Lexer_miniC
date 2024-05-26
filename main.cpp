#include "iostream"
#include "sstream"
#include "string"
#include "lexem.h"
#include "ExprSA_test.h"
#include "TopDownSA.h"

using namespace std;



int main(){
//    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\lex-lib\\input_3.txt)");
//    Lexer lexer(file);
//    pair<string, string> lexem;
//
//
//    while (true){
//        lexem = lexer.getNextLexem();
//        cout << "[" << lexem.first << ", \"" << lexem.second << "\"]\n";
//        if (lexem.first == "EOF" or lexem.first == "error"){
//            break;
//        }
//    }
    fstream file(R"(..\\input.txt)");
    string treefile = R"(..\\outputTree.txt)";
    string atomfile = R"(..\\outputAtoms.txt)";
    string asmfile = R"(..\\outputAsm.txt)";
    Lexer lexer = Lexer(file);
    LL ll = LL(lexer,  treefile, atomfile, asmfile);
    ll.solve();
    return 0;
}