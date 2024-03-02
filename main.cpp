#include <iostream>
#include <fstream>
#include <string>
#include "Lexer_lib/Lexer.h"
#include "shunting_yard.h"


using namespace std;

int main(){
    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\Lexer_lib\\input_3.txt)");
    Lexer lexer(file);
    pair<string, string> lexem;


    while (true){
        lexem = lexer.getNextLexem();
        cout << "[" << lexem.first << ", \"" << lexem.second << "\"]\n";
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
    }

    /*
    Shunting_yard SY = Shunting_yard();
    while (true){
        lexem = lexer.getNextLexem();
        if (lexem.first == "error") {
            cout << "[error, \"\"" << endl;
            break;
        }
        vector<pair<string , string>> list = lexem.first == "EOF"? SY.clear_stack(): SY.next_token(lexem);
        for (auto token: list) {
            if (token.first == "error") {
                lexem = token;
                break;
            }
            cout << "[" << token.first << ", \"" << token.second << "\"]\n";
        }
        if (lexem.first == "EOF" or lexem.first == "error"){
            cout << "[" << lexem.first << ", \"" << lexem.second << "\"]\n";
            break;
        }
    }
     */

    return 0;
}