#include <iostream>
#include "graph.h"
#include "check.h"
#include <fstream>

using namespace std;

struct E {
    int newState;
    bool append;
    bool read;
    string lexem;
    bool clear = false;
};

class Lexer{
private:
    ifstream file;
    string buffer;
    int newState = 0;
    string cache;
    unordered_map<int, unordered_map<string, E>> graph = getGraph();

public:

    Lexer() = default;
    pair<string, string > GetNextLexem(){
        char ch;
        if (file.peek() == EOF){
            return {"EOF", ""};
        }
        file.get(ch);
        string cache(1, ch);
        buffer = "";
        while (newState >= 0){
            //cout << newState << " {" << cache << "}" << endl;
            if (graph[newState].contains(cache)){
                //newState = graph[newState][cache].newState;
                if (graph[newState][cache].append){
                    buffer += cache;
                }
                if (graph[newState][cache].lexem != ""){
                    int state = newState;
                    newState = graph[newState][cache].newState;
                    return {graph[state][cache].lexem, buffer};
                }
                string str = cache;
                if (graph[newState][cache].read){
                    if (file.get(ch)){
                        string new_cache(1, ch);
                        cache = new_cache;
                    } else {
                        newState = -1;
                    }
                }
                newState = graph[newState][str].newState;

            }
            else if (graph[newState].contains("letter")){
                //cout << newState << " {" << cache << "}" << endl;
                if (is_letter(cache)){
                    if (graph[newState]["letter"].append){
                        buffer += cache;
                    }
                    if (graph[newState]["letter"].lexem != ""){
                        int state = newState;
                        newState = graph[newState]["letter"].newState;
                        return {graph[state]["letter"].lexem, buffer};
                    }
                    if (graph[newState]["letter"].read){
                        if (file.get(ch)){
                            string new_cache(1, ch);
                            cache = new_cache;
                        } else {
                            newState = -1;
                        }
                    }
                    newState = graph[newState]["letter"].newState;
                }

            }

            else if (graph[newState].contains("digit")){
                if (is_digit(cache)){
                    if (graph[newState]["digit"].append){
                        buffer += cache;
                    }
                    if (graph[newState]["digit"].lexem != ""){
                        int state = newState;
                        newState = graph[newState]["digit"].newState;
                        return {graph[state]["digit"].lexem, buffer};
                    }
                    if (graph[newState]["digit"].read){
                        if (file.get(ch)){
                            string new_cache(1, ch);
                            cache = new_cache;
                        } else {
                            newState = -1;
                        }
                    }
                    newState = graph[newState]["digit"].newState;
                }

            }
            else if (graph[newState].contains("any")){
                if (is_any(cache)){
                    if (graph[newState]["any"].append){
                        buffer += cache;
                    }
                    if (graph[newState]["any"].lexem != ""){
                        int state = newState;
                        newState = graph[newState]["any"].newState;
                        return {graph[state]["any"].lexem, buffer};
                    }
                    if (graph[newState]["any"].read){
                        if (file.get(ch)){
                            string new_cache(1, ch);
                            cache = new_cache;
                        } else {
                            newState = -1;
                        }
                    }
                    newState = graph[newState]["any"].newState;
                }

            }

            else if (graph[newState].contains("whitespace")){
                cout << "space" << endl;
                if (is_whitespace(cache)){
                    if (file.peek() != EOF){
                        file.get(ch);
                        string new_cache(1, ch);
                        cache = new_cache;
                    } else {
                        newState = -1;
                    }
                    newState = graph[newState]["whitespace"].newState;
                }
            }

            else {
                if (graph[newState]["default"].append){
                    buffer += cache;
                }
                if (graph[newState]["default"].lexem != ""){
                    int state = newState;
                    newState = graph[newState]["default"].newState;
                    return {graph[state]["default"].lexem, buffer};
                }
                if (graph[newState]["default"].read){
                    if (file.get(ch)){
                        string new_cache(1, ch);
                        cache = new_cache;
                    } else {
                        newState = -1;
                    }
                }
                newState = graph[newState]["default"].newState;
            }
        }
        return {"error", ""};
    }

    void setfile(const string &direct){
        file.open(direct);
        return;
    }

    void printFile(){
        char c;
        while(file.get(c)){
            cout << c;
        }
        return;
    }
};

int main(){
    Lexer lex;
    lex.setfile("D:\\files\\solution_for_clion\\LexminiC\\input.txt");

    while (true) {
        auto lexem = lex.GetNextLexem();
        cout << "[" << lexem.first << ", \"" << lexem.second << "\"]" << endl;
        if (lexem.first == "EOF" or lexem.first == "error"){
            break;
        }
    }
    return 0;
}
