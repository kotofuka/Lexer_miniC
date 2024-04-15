cmake_minimum_required(VERSION 3.14)
project(lex-lib)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)




add_library(SA-lib scr/ExprSA_test.cpp scr/TopDownSA.cpp)
target_link_libraries(SA-lib lex-lib)
target_include_directories(SA-lib PUBLIC include)