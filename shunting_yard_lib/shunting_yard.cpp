#include "shunting_yard.h"
#include <iostream>
vector<pair<string, string>> Shunting_yard::next_token(const pair<std::string, std::string> & token) {
    //cout << "| [" << token.first << ", \"" << token.second << "\"] |\n";
    if (token.first == "num") return {token};
    else if (dict.contains(token)){
        vector <pair<string, string>> list;
        if (token.first == "rpar"){
            while (!stack.empty() and stack.top().first != "lpar"){
                list.push_back(stack.top());
                stack.pop();
            }
            if (stack.empty()) return {{"error", ""}};
            stack.pop();
        }
        else if (stack.empty()) stack.push(token);
        else if (dict[token] > dict[stack.top()]) stack.push(token);
        else{
            while (!stack.empty() and stack.top().first != "lpar"){
                list.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        return list;
    }
    else return {{"error", ""}};
}

vector<pair<string, string>> Shunting_yard::clear_stack() {
    vector<pair<string, string>> list;
    while (!stack.empty()){
        list.push_back(stack.top());
        stack.pop();
    }
    return list;
}

Shunting_yard::Shunting_yard() {};