project(shunting_yard_lib)

set(HEADER_FILES shunting_yard.h)

set(SOURCE_FILES shunting_yard.cpp)

add_library(shunting_yard_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})