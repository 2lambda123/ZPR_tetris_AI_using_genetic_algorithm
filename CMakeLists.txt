cmake_minimum_required(VERSION 3.15)

project(genetic-tetris)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4)
else()
    # lots of warnings
    add_compile_options(-Wall -Wextra -pedantic -g)
endif()

if (UNIX)
    find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
elseif (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    if (NOT CMAKE_CL_64)
        set(SFML_DIR "lib/win/32bit/SFML-2.5.1/lib/cmake/SFML")
    else()
        set(SFML_DIR "lib/win/64bit/SFML-2.5.1/lib/cmake/SFML")
    endif()

    find_package(SFML 2.5.1 COMPONENTS graphics system window REQUIRED)

    set(Boost_USE_STATIC_LIBS ON) 
    find_package(Boost 1.67 COMPONENTS unit_test_framework REQUIRED)
    message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost library dir: ${Boost_LIBRARY_DIRS}")
    link_directories(${Boost_LIBRARY_DIRS})
endif()

set(TETRIS_SOURCES project/src/tetris.cpp
            project/src/tetromino.cpp
            project/src/tetromino_generator.cpp
            )
set(APP_SOURCES project/src/app.cpp
                project/src/gui.cpp)

include_directories(project/include)
include_directories(lib/common/include)

add_executable(test_catch2 project/tests/test_catch2.cpp)
add_executable(test_boost project/tests/test_boost.cpp)
add_executable(test_sfml project/tests/test_sfml.cpp)
add_executable(test_tetromino ${TETRIS_SOURCES} project/tests/test_tetromino.cpp)
add_executable(app ${TETRIS_SOURCES} ${APP_SOURCES} project/src/main.cpp)

target_link_libraries(app sfml-graphics sfml-system sfml-window)
target_link_libraries(test_sfml sfml-graphics)

if (UNIX)
    target_link_libraries(test_boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
elseif (WIN32)
    target_link_libraries(test_boost Boost::unit_test_framework)
endif()

# copy /res folder to a folder containing binary
add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:app>/res
        )

