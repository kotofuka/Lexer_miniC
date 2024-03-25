cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)

add_executable(
        Google_tests
        lex-tests.cpp
        shunting-yard-tests.cpp
)
target_link_libraries(
        Google_tests
        lex-lib
        shunting-yard-lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Google_tests)