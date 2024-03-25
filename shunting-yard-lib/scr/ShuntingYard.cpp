#include "ShuntingYard.h"
#include <iostream>
#include <vector>


using namespace std;

ShuntingYard::ShuntingYard(std::istream& stream): lexer(stream) {
    while (!output.empty()) output.pop();
    while (!stack.empty()) stack.pop();
}

void ShuntingYard::processLexem(const Lexem & token) {
    //cout << token.first << endl;
    if (token.first == "num" or token.first == "id"){
        if (!flag) { output.push(LEX_ERROR);
            //cout << 1 << endl;
        }
        else{
            flag = false;
            output.push(token);
        }
    }
    else if (token.first == "lpar"){
        if (!flag) { output.push(LEX_ERROR);
            //cout << 2 << endl;
            }
        else{
            flag = true;
            stack.push(token);
        }
    }
    else if (token.first == "rpar"){
        if (flag) { output.push(LEX_ERROR);
            //cout << 3 << endl;
        }
        else {
            flag = false;
            while (!stack.empty() and stack.top().first != "lpar"){
                output.push(stack.top());
                //cout << "| " << stack.top().first << " |" << endl;
                stack.pop();
            }
            if (stack.empty()){
                //cout << 4 << endl;
                output.push(LEX_ERROR);
            }
            else {
                stack.pop();
            }
        }
    }
    else if (dict.contains(token.first)){

        if (flag) { output.push(LEX_ERROR);
            //cout << 5 << endl;
        }
        else{
            flag = true;

            while (!stack.empty() and dict[stack.top().first] >= dict[token.first] and stack.top().first != "lpar"){
                output.push(stack.top());
                stack.pop();
            }

            stack.push(token);
        }
    }
    else if (token == LEX_ERROR or token == LEX_EOF){
        if (flag or token == LEX_ERROR) { output.push(LEX_ERROR);
            //cout << 6 << endl;
        }
        else {
            while (!stack.empty() and stack.top().first != "lpar"){
                output.push(stack.top());
                stack.pop();
            }
            if (!stack.empty()) { output.push(LEX_ERROR);
                //cout << 7 << endl;
            }
            else {
                output.push(token);
            }
        }
    }
}

Lexem ShuntingYard::getNextLexem()
{
    // TODO: implement shunting yard
    while (output.empty()){
        processLexem(lexer.getNextLexem());
    }

    if (output.front() == LEX_ERROR){
        return output.front();
    }

    Lexem token = output.front();
    output.pop();
    return token;
}