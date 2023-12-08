//
// Created by Admin on 07.12.2023.
//

#include "check.h"
#include <cctype>
//#include <iostream>

using namespace std;

bool is_letter(string str){
    if (isalpha(str[0])){
        return true;
    }
    return false;
}

bool is_digit(string str){
    if (isalnum(str[0])){
        return true;
    }
    return false;
}

bool is_whitespace(string str){
    //std::cout << (str[0] == ' ' or str[0] == '\t' or str[0] == '\n') << std::endl;
    if (str[0] == ' ' or str[0] == '\t' or str[0] == '\n'){
        return true;
    }
    return false;
}

bool is_any(std::string str){
    if (str != ""){
        return true;
    }
    return false;
}