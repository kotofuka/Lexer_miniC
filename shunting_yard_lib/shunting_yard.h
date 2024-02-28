//
// Created by Admin on 28.02.2024.
//

#ifndef LEXMINIC_SHUNTING_YARD_H
#define LEXMINIC_SHUNTING_YARD_H

#include <string>
#include <stack>
#include <map>
#include <vector>

using namespace std;

class Shunting_yard{
private:
    map <pair<string, string>, int> dict = {{{"opplus", ""}, 1}, {{"opminus", ""}, 1},
                                            {{"opmul", ""}, 2},
                                            {{"lpar", ""}, 3}, {{"rpar", ""}, 3}};
    stack<pair<string, string>> stack;

public:
    Shunting_yard();

    vector<pair<string, string>> clear_stack();

    vector<pair<string, string>> next_token(const pair<string, string>&);
};

#endif //LEXMINIC_SHUNTING_YARD_H
