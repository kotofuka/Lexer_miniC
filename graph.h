//
// Created by Admin on 06.12.2023.
//

#ifndef LEXMINIC_GRAPH_H
#define LEXMINIC_GRAPH_H

#include <string>
#include <unordered_map>

using namespace std;

const string SPACE = " ";
const string EMPTY = "";



struct E;

unordered_map<int, unordered_map<string, E>> getGraph();
#endif //LEXMINIC_GRAPH_H
