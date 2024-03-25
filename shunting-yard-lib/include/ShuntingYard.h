//
// Created by Admin on 28.02.2024.
//

#ifndef LEXMINIC_SHUNTINGYARD_H
#define LEXMINIC_SHUNTINGYARD_H

#include "Lexer.h"
#include "graph.h"
#include <iostream>
#include <unordered_map>
#include <stack>
#include <queue>

class ShuntingYard {
public:
    ShuntingYard(std::istream& stream);
    Lexem getNextLexem();

protected:
private:
    Lexer lexer;
    bool flag = true;
    std::unordered_map<std::string, int> dict = {{"opmul", 2},
                                                 {"opplus", 1}, {"opminus", 1}};

    void processLexem (const Lexem&);

    std::stack<Lexem> stack;
    std::queue<Lexem> output;
};
#endif //LEXMINIC_SHUNTINGYARD_H
