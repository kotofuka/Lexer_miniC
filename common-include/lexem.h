//
// Created by Admin on 04.03.2024.
//

#ifndef LEXMINIC_LEXEM_H
#define LEXMINIC_LEXEM_H

#include <iostream>
#include <string>
#include <vector>

using Lexem = std::pair<std::string, std::string>;

const Lexem LEX_EMPTY = { "", "" };
const Lexem LEX_ERROR = { "error", "" };
const Lexem LEX_EOF = { "EOF", "" };

#endif //LEXMINIC_LEXEM_H
