cmake_minimum_required(VERSION 3.14)
project(lex-lib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(lex-lib src/graph.cpp src/Lexer.cpp)
target_include_directories(lex-lib PUBLIC include)