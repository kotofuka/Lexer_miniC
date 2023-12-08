cmake_minimum_required(VERSION 3.26)
project(LexminiC)

set(CMAKE_CXX_STANDARD 20)

add_executable(LexminiC main.cpp
        graph.cpp
        graph.h
        check.cpp
        check.h)
