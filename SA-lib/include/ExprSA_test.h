//
// Created by Admin on 22.03.2024.
//

#ifndef LEX_TESTS_EXPRSA_H
#define LEX_TESTS_EXPRSA_H

#include <vector>
#include <map>
#include <string>
#include <lexem.h>
#include <Lexer.h>

class Expr_SA{
public:
    Expr_SA(std::istream& stream);
    std::vector<Lexem> getLRTokens();
    bool getResult();

private:
    Lexer lexer;
    std::vector<Lexem> result_list;
    std::map <std::vector<Lexem>, Lexem> dict;
    bool result = false;

    std::vector<Lexem> findKey(const std::vector<Lexem>& list);
    bool tokenListContains(const std::vector<Lexem>& tokenList, const std::vector<Lexem>& vector);

    void processTokens();



};

#endif //LEX_TESTS_EXPRSA_H
