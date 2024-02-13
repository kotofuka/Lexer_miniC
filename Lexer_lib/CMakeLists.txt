project(Lexer_lib)

set(HEADER_FILES Lexer.h graph.h)

set(SOURCE_FILES Lexer.cpp graph.cpp)

add_library(Lexer_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})