//
// Created by Admin on 06.12.2023.
//

#include "graph.h"

struct E {
    int newState;
    bool append;
    bool read;
    string lexem;
    bool clear = false;
};

unordered_map<int, unordered_map<string, E>> getGraph() {
    unordered_map<int, unordered_map<string, E>> graph = {
            {0, {{"whitespace", {0, false, true, EMPTY}},

                 // state 1
                 {"(", {0, false, true, "lpar"}},
                 {")", {0, false, true, "rpar"}},
                 {"{", {0, false, true, "lbrace"}},
                 {"}", {0, false, true, "rbrace"}},
                 {";", {0, false, true, "semicolon"}},
                 {",", {0, false, true, "comma"}},
                 {".", {0, false, true, "colon"}},
                 {">", {0, false, true, "opgt"}},
                 {"*", {0, false, true, "opmul"}},

                 // state 2
                 {"<", {2, false, true, EMPTY}},

                 // state 4
                 {"!", {4, false, true, EMPTY}},
                 // state 6
                 {"=", {6, false, true, EMPTY}},
                 // state 8
                 {"+", {8, false, true, EMPTY}},
                 // state 10
                 {"|", {10, false, true, EMPTY}},
                 // state 12
                 {"&", {12, false, true, EMPTY}},
                 // state 14
                 {"\'", {14, false, true, EMPTY}},
                 // state 18
                 {"\"", {18, false, true, EMPTY}},
                 // state 21
                 {"letter", {21, true, true, EMPTY}},
                 }},

            {2, {{"=", {0, false, true, "ople"}},
                 {"default", {0, false, false, "oplt"}}
            }},

            {4, {{"=", {0, false, true, "opne"}},
                        {"default", {0, false, false, "opnot"}}
            }},

            {6, {{"=", {0, false, true, "opeq"}},
                        {"default", {0, false, false, "opassing"}}
            }},

            {8, {{"+", {0, false, true, "opinc"}},
                        {"default", {0, false, false, "opplus"}}
            }},

            {10, {{"|", {0, false, true, "opor"}},
                        {"default", {-1, false, false, "error"}}
            }},

            {12, {{"&", {0, false, true, "opand"}},
                         {"default", {-1, false, false, "error"}}
            }},

            {14, {{"any", {16, true, true, EMPTY}},
                         {"\'", {-1, false, false, "error"}}
            }},

            {16, {{"\'", {0, false, true, "char"}},
                         {"default", {-1, false, false, "error"}}
            }},

            {18, {{"any", {18, true, true, EMPTY}},
                         {"\"", {0, false, true, "str"}}
            }},

            {21, {{"digit", {21, true, true, EMPTY}},
                         {"letter", {21, true, true, EMPTY}},
                         {"keyword", {0, false, true, "keyword"}},
                         {"default", {0, false, true, "id"}}
            }}
    };

    return graph;
}
