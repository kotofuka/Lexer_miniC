cmake_minimum_required(VERSION 3.14)
project(lex-lib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(shunting-yard-lib scr/ShuntingYard.cpp)
target_link_libraries(shunting-yard-lib lex-lib)
target_include_directories(shunting-yard-lib PUBLIC include)