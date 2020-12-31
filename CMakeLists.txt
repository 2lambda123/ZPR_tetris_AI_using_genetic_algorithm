cmake_minimum_required(VERSION 3.15)

project(genetic-tetris)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # lots of warnings and debugging
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

include_directories(project/include)
include_directories(lib/common/include)

add_library(tetris-lib project/include/tetris/tetris.hpp
        project/src/tetris/tetris.cpp
        project/include/tetris/tetromino.hpp
        project/src/tetris/tetromino.cpp
        project/include/tetris/tetromino_generator.hpp
        project/src/tetris/tetromino_generator.cpp
)

add_library(gui-lib
        project/include/app.hpp
        project/src/app.cpp
        project/include/gui/gui.hpp
        project/src/gui/gui.cpp project/include/gui/utils.hpp project/src/gui/utils.cpp)

add_library(ai-lib
        project/include/AI/ai.hpp
        project/include/AI/evolutionary_strategy.hpp
        project/src/AI/evolutionary_strategy.cpp
        project/include/AI/move.hpp project/src/AI/move.cpp
        project/include/AI/genome.hpp
        project/src/AI/genome.cpp)

target_link_libraries(gui-lib sfml-system sfml-graphics sfml-window sfml-audio)
target_link_libraries(ai-lib tetris-lib)

add_subdirectory(tests)

add_executable(app project/src/main.cpp)

target_link_libraries(app gui-lib ai-lib pthread)

# copy /res folder to a folder containing binary
add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:app>/res
        )

