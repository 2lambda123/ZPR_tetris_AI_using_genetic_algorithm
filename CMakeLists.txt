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
    find_package(SFML 2.5 COMPONENTS graphics system window audio REQUIRED)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
elseif (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    if (NOT CMAKE_CL_64)
        set(SFML_DIR "lib/win/32bit/SFML-2.5.1/lib/cmake/SFML")
    else()
        set(SFML_DIR "lib/win/64bit/SFML-2.5.1/lib/cmake/SFML")
    endif()

    find_package(SFML 2.5.1 COMPONENTS graphics system window audio REQUIRED)

    set(Boost_USE_STATIC_LIBS ON) 
    find_package(Boost 1.67 COMPONENTS unit_test_framework REQUIRED)
    message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost library dir: ${Boost_LIBRARY_DIRS}")
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories(project/include)
include_directories(lib/common/include)

add_library(tetris-lib
        project/src/tetris/tetris.cpp
        project/src/tetris/tetromino.cpp
        project/src/tetris/tetromino_generator.cpp
        project/src/tetris/wall_kicks.cpp)

add_library(gui-lib
        project/src/app.cpp
        project/src/gui/gui.cpp
        project/src/gui/gui_utils.cpp
        project/src/gui/screen/game_screen.cpp
        project/src/gui/screen/menu_screen.cpp
        project/src/gui/screen/evolve_screen.cpp
        project/src/controller/game_controller.cpp
        project/src/controller/evolve_controller.cpp)

add_library(ai-lib
        project/src/AI/evolutionary_strategy.cpp
        project/src/AI/move.cpp
        project/src/AI/genome.cpp
        project/src/AI/ai_utils.cpp)

target_link_libraries(gui-lib sfml-system sfml-graphics sfml-window sfml-audio)
target_link_libraries(ai-lib tetris-lib)

add_subdirectory(tests)

add_executable(app project/src/main.cpp)

if (UNIX)
    target_link_libraries(app gui-lib ai-lib pthread)
elseif (WIN32)
    target_link_libraries(app gui-lib ai-lib)
endif ()


# copy /res folder to a folder containing binary
add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:app>/res
        )

