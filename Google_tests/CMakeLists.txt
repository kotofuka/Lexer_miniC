project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_test_run lex-tests.cpp)

target_link_libraries(Google_test_run Lexer_lib)

target_link_libraries(Google_test_run gtest gtest_main)