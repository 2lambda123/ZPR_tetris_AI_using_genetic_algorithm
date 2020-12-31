add_executable(test_sfml functional/test_sfml.cpp)
add_executable(tetris_no_gui functional/tetris_no_gui.cpp)
add_executable(tetris_gui_integration functional/tetris_gui_integration.cpp)
add_executable(test_boost unit/test_boost.cpp)
add_executable(test_tetris unit/test_tetris.cpp)
add_executable(test_ai unit/test_ai.cpp)

include_directories(project/include)

target_link_libraries(test_sfml sfml-graphics)
target_link_libraries(tetris_gui_integration gui-lib)

target_link_libraries(test_boost Boost::unit_test_framework)
target_link_libraries(test_tetris Boost::unit_test_framework)
target_link_libraries(test_ai Boost::unit_test_framework ai-lib)

target_link_libraries(tetris_no_gui tetris-lib)
target_link_libraries(tetris_gui_integration tetris-lib)
target_link_libraries(test_tetris tetris-lib)

# copy /res folder to a folder containing binary
add_custom_command(TARGET tetris_gui_integration POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:tetris_gui_integration>/res
        )