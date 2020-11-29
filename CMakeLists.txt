cmake_minimum_required(VERSION 3.15)

project(genetic-tetris)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings
    add_compile_options(-Wall -Wextra -pedantic -g)
endif()

if (UNIX)
    find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
elseif (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "lib/win/SFML-2.5.1/lib/cmake/SFML")
    find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED) 

    set(Boost_USE_STATIC_LIBS ON) 
    find_package(Boost 1.67 COMPONENTS unit_test_framework REQUIRED)
    message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost library dir: ${Boost_LIBRARY_DIRS}")
    link_directories(${Boost_LIBRARY_DIRS})
endif()

set(SOURCES project/src/tetris.cpp
            project/src/tetromino.cpp
            project/src/tetromino_generator.cpp)

include_directories(project/include)
include_directories(lib/common/include)

add_executable(test_catch2 project/tests/test_catch2.cpp)
add_executable(test_boost project/tests/test_boost.cpp)
add_executable(hello_sfml project/tests/hello_sfml.cpp)
add_executable(tetris ${SOURCES} project/src/main.cpp)

if (UNIX)
    target_link_libraries(test_boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    target_link_libraries(hello_sfml sfml-graphics)
endif()

if (WIN32)
    target_link_libraries(hello_sfml sfml-graphics)
    # target_link_libraries(test_boost libboost_unit_test_framework-vc141-mt-gd-x64-1_67)
    target_link_libraries(test_boost Boost::unit_test_framework)
    # target_link_libraries(test_boost unit_test_framework)
    # linker nie moze znalezc boost_unit_test_framework-vc141-mt-gd-x64-1_67.lib 
endif()
