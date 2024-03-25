#include "iostream"
#include "sstream"
#include "string"
#include "lexem.h"
#include "ExprSA_test.h"

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
    string expression = "a+1";

    stringstream stream { expression };
    Expr_SA exprSa (stream);

    vector<Lexem> tokenList = exprSa.getLRTokens();

    for (const auto& token: tokenList){
        cout << "{\"" << token.first << "\", \"" << token.second << "\"}" << endl;
    }

    return 0;
}