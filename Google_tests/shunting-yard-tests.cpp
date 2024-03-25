//
// Created by sr9000 on 02/03/24.
//
#include "gtest/gtest.h"
#include "Lexer.h"
#include "ShuntingYard.h"
#include <sstream>
#include <string>
#include <vector>

namespace shunting_yard_tests {

TEST(CorrectExpression, OnePlusTwo)
{
    using namespace std;
    string expression = "1+2";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "opplus", "" },
        LEX_EOF,
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(CorrectExpression, TheLongOne)
{
    using namespace std;
    string expression = "11+222*3333- 44444";
    vector<Lexem> correct = {
        { "num", "11" },
        { "num", "222" },
        { "num", "3333" },
        { "opmul", "" },
        { "opplus", "" },
        { "num", "44444" },
        { "opminus", "" },
        LEX_EOF,
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(CorrectExpression, TheLongOneWithParentheses)
{
    using namespace std;
    string expression = "55*(11+22)*(66+33*(0- 44))";
    vector<Lexem> correct = {
        { "num", "55" },
        { "num", "11" },
        { "num", "22" },
        { "opplus", "" },
        { "opmul", "" },
        { "num", "66" },
        { "num", "33" },
        { "num", "0" },
        { "num", "44" },
        { "opminus", "" },
        { "opmul", "" },
        { "opplus", "" },
        { "opmul", "" },
        LEX_EOF,
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(VariablesInExpression, OnePlusA)
{
    using namespace std;
    string expression = "1+a";
    vector<Lexem> correct = {
        { "num", "1" },
        { "id", "a" },
        { "opplus", "" },
        LEX_EOF,
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(VariablesInExpression, LongWithVariables)
{
    using namespace std;
    string expression = "1+a*(4-b*2)+5";
    vector<Lexem> correct = {
        { "num", "1" },
        { "id", "a" },
        { "num", "4" },
        { "id", "b" },
        { "num", "2" },
        { "opmul", "" },
        { "opminus", "" },
        { "opmul", "" },
        { "opplus", "" },
        { "num", "5" },
        { "opplus", "" },
        LEX_EOF,
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(BadParentheses, UnClosed)
{
    using namespace std;
    string expression = "1+(2*a+3";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "id", "a" },
        { "opmul", "" },
        { "num", "3" },
        { "opplus", "" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(BadParentheses, UnOpened)
{
    using namespace std;
    string expression = "(1+2)*a)+(3";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "opplus", "" },
        { "id", "a" },
        { "opmul", "" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(TooFewArgumens, StartsFromOp)
{
    using namespace std;
    string expression = "+ 1 + 2";
    vector<Lexem> correct = {
        { "num", "1" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(TooFewArgumens, NoOperand)
{
    using namespace std;
    string expression = "1+2*3*";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "num", "3" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(TooMuchArgumens, DoubleExpression)
{
    using namespace std;
    string expression = "1+2 3+4";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "opplus", "" },
        { "num", "3" },
        { "num", "4" },
        { "opplus", "" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(TooMuchArgumens, ExtraArgument)
{
    using namespace std;
    string expression = "1 2 + 3";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        { "opplus", "" },
        { "num", "3" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(NotAnExpression, NotInfix)
{
    using namespace std;
    string expression = "1 2 + * 3";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

TEST(NotAnExpression, NotInfixWithPrantheses)
{
    using namespace std;
    string expression = "1 2+(*3)";
    vector<Lexem> correct = {
        { "num", "1" },
        { "num", "2" },
        LEX_ERROR
    };

    stringstream stream { expression };
    ShuntingYard shYard(stream);

    for (auto&& lex : correct) {
        EXPECT_EQ(lex, shYard.getNextLexem());
    }
}

}