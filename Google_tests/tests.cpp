#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include "Lexer.h"
#include "vector"

using namespace std;

vector <pair<string, string>> case1 = {{"id", "a"},
                                      {"opplus", ""},
                                      {"id", "b"},
                                      {"EOF", ""},
                                      };


vector <pair<string, string>> case2 = {{"id", "a"},
                                       {"opminus", ""},
                                       {"id", "b"},
                                       {"EOF", ""},
};


vector <pair<string, string>> case3 = {{"kwint", ""},
                                        {"id", "main"},
                                        {"lpar", ""},
                                        {"rpar", ""},
                                        {"lbrace", ""},
                                        {"kwint", ""},
                                        {"id", "a"},
                                        {"opassign", ""},
                                        {"num", "1"},
                                        {"semicolon", ""},
                                        {"kwint", ""},
                                        {"id", "b"},
                                        {"opassign", ""},
                                        {"num", "4"},
                                        {"semicolon", ""},
                                        {"kwint", ""},
                                        {"id", "c"},
                                        {"opassign", ""},
                                        {"id", "a"},
                                        {"opminus", ""},
                                        {"id", "b"},
                                        {"semicolon", ""},
                                        {"rbrace", ""},
                                        {"EOF", ""},};


TEST(TokenCaseSuit, Test1){
    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\Lexer_lib\\input_1.txt)");
    Lexer lexer(file);
    pair<string, string> lexem;
    vector <pair<string, string>> token_list = case1;
    int i = 0;
    while (true){
        lexem = lexer.getNextLexem();
        EXPECT_EQ(lexem, token_list[i]);
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
        i++;
    }
}

TEST(TokenCaseSuit, Test2){
    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\Lexer_lib\\input_2.txt)");
    Lexer lexer(file);
    pair<string, string> lexem;
    vector <pair<string, string>> token_list = case2;
    int i = 0;
    while (true){
        lexem = lexer.getNextLexem();
        EXPECT_EQ(lexem, token_list[i]);
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
        i++;
    }
}


TEST(TokenCaseSuit, Test3){
    fstream file(R"(D:\\files\\solution_for_clion\\Lexer_miniC\\Lexer_lib\\input_3.txt)");
    Lexer lexer(file);
    pair<string, string> lexem;
    vector <pair<string, string>> token_list = case3;
    int i = 0;
    while (true){
        lexem = lexer.getNextLexem();
        EXPECT_EQ(lexem, token_list[i]);
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
        i++;
    }
}