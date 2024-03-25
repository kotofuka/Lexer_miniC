//
// Created by Admin on 08.12.2023.
//

#ifndef LEXMINIC_GRAPH_H
#define LEXMINIC_GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>

struct E{
    std::string filter;
    int newState;
    bool append;
    bool read;
    std::string lexem;
    bool clear = false;
};

std::unordered_map<int, std::vector<E>> getGraph();

#endif //LEXMINIC_GRAPH_H
