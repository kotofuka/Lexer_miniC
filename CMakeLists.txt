cmake_minimum_required(VERSION 3.14)
project(lex_tests)

set(CMAKE_CXX_STANDARD 20)

include_directories(common-include)

set(SOURCE_FILES main.cpp)
add_executable(main_run ${SOURCE_FILES})
add_executable(main_run_exp ${SOURCE_FILES})

include_directories(shunting-yard-lib)

include_directories(SA-lib)


add_subdirectory(lex-lib)
add_subdirectory(shunting-yard-lib)
add_subdirectory(SA-lib)
add_subdirectory(Google_tests)

target_link_libraries(main_run shunting-yard-lib)
target_link_libraries(main_run_exp SA-lib)