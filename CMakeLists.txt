cmake_minimum_required(VERSION 3.26)
project(LexminiC)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)
add_executable(Lexer_run ${SOURCE_FILES})

include_directories(Lexer_lib)
add_subdirectory(Lexer_lib)

target_link_libraries(Lexer_run Lexer_lib)

add_subdirectory(Google_tests)
