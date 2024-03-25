//
// Created by Admin on 22.03.2024.
//
#include <ExprSA_test.h>
std::map <std::vector<Lexem>, Lexem> grammer = {
        //E
        {{{"E7", ""}}, {"E", ""}},

        //E7
        {{{"E7", ""}, {"opor", ""}, {"E6", ""}}, {"E7", ""}},
        {{{"E6", ""}}, {"E7", ""}},

        //E6
        {{{"E5", ""}}, {"E6", ""}},
        {{{"E6", ""}, {"opand", ""}, {"E5", ""}}, {"E6", ""}},

        //E5
        {{{"E4", ""}, {"opeq", ""}, {"E4", ""}}, {"E5", ""}},
        {{{"E4", ""}, {"opne", ""}, {"E4", ""}}, {"E5", ""}},
        {{{"E4", ""}, {"opgt", ""}, {"E4", ""}}, {"E5", ""}},
        {{{"E4", ""}, {"oplt", ""}, {"E4", ""}}, {"E5", ""}},
        {{{"E4", ""}, {"ople", ""}, {"E4", ""}}, {"E5", ""}},
        {{{"E4", ""}}, {"E5", ""}},

        //E4
        {{{"E4", ""}, {"opplus", ""}, {"E3", ""}}, {"E4", ""}},
        {{{"E4", ""}, {"opminus", ""}, {"E3", ""}}, {"E4", ""}},
        {{{"E3", ""}}, {"E4", ""}},

        //E3
        {{{"E3", ""}, {"opmul", ""}, {"E2", ""}}, {"E3", ""}},
        {{{"E2", ""}}, {"E3", ""}},

        //E2
        {{{"opnot", ""}, {"E1", ""}}, {"E2", ""}},
        {{{"E1", ""}}, {"E2", ""}},

        //E1
        {{{"opinc", ""}, {"id", ""}}, {"E1", ""}},
        {{{"id", ""}, {"opinc", ""}}, {"E1", ""}},
        {{{"lpar", ""}, {"E", ""}, {"rpar", ""}}, {"E1", ""}},
        {{{"num", ""}}, {"E1", ""}},
        {{{"id", ""}}, {"E1", ""}},
};


Expr_SA::Expr_SA(std::istream &stream): lexer(stream) {
    Lexem token;
    dict = grammer;

    while (token != LEX_EOF and token != LEX_ERROR){
        token = lexer.getNextLexem();

        if (token != LEX_EOF) result_list.push_back(token);
        else break;

        processTokens();
    }
    std::vector<Lexem> list = getLRTokens();
    std::vector<Lexem> prev_list;
     do {
         prev_list = list;
         processTokens();
         list = getLRTokens();
     } while (list != prev_list);

    if (list[0].first == "E") result = true;
    else result = false;
    return;
}

void Expr_SA::processTokens() {
    auto iter = result_list.begin();

    while(true){

        if (iter == result_list.end()) return;
        auto key = findKey(std::vector<Lexem>(iter, result_list.end()));

        if (!key.empty()){
            result_list.erase(iter, result_list.end());

            result_list.insert(iter, dict[key]);
            std::cout<< "==============================" << std::endl;
            for (const auto& token: result_list){
                std::cout << "{\"" << token.first << "\", \"" << token.second << "\"}" << std::endl;
            }
            std::cout<< "==============================" << std::endl;
            return;
        }

        iter++;
    }
}

std::vector<Lexem> Expr_SA::getLRTokens() {
    return result_list;
}

std::vector<Lexem> Expr_SA::findKey(const std::vector<Lexem> &list) {
    for (const auto& token_list: dict){
        if (tokenListContains(token_list.first, list)){
            return token_list.first;
        }
    }
    return {};
}

bool Expr_SA::tokenListContains(const std::vector<Lexem> &tokenList, const std::vector<Lexem> &vector) {
    if (tokenList.size() != vector.size()) return false;

    for (size_t i = 0; i < tokenList.size(); i++){
        if (tokenList[i].first != vector[i].first) return false;
    }
    return true;
}

bool Expr_SA::getResult() {return result;}

