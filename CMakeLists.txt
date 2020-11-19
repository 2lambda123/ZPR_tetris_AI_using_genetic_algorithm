cmake_minimum_required(VERSION 3.1)

project(genetic-tetris)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

set(SOURCES project/src/func.cpp)

include_directories(project/include)
include_directories(lib/include)

add_executable(hello ${SOURCES} project/src/helloworld.cpp)
add_executable(test_catch2 project/tests/test_catch2.cpp)
add_executable(test_boost project/tests/test_boost.cpp)
add_executable(hello_sfml project/tests/hello_sfml.cpp)

target_link_libraries(test_boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_link_libraries(hello_sfml sfml-graphics)
