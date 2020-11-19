cmake_minimum_required(VERSION 3.1)

project(genetic-tetris)

# file(GLOB SOURCES "project/src/*.cpp" "project/include/*.hpp")
set(TESTS project/src/catch_amalgamated.cpp project/tests/test.cpp)
set(SOURCES project/src/func.cpp)

include_directories(project/include)

add_executable(hello ${SOURCES} project/src/helloworld.cpp)
add_executable(tests ${SOURCES} project/tests/test.cpp)
