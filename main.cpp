#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"

using namespace std;

int main(){
    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\input.txt)");
    Lexer lexer(file);
    pair<string, string> lexem;

    while (true){
        lexem = lexer.getNextLexem();
        cout << "[" << lexem.first << ", \"" << lexem.second << "\"]\n";
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
    }

    return 0;
}