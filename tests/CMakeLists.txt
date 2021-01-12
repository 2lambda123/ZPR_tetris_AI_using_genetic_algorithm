add_executable(test_sfml functional/test_sfml.cpp)
add_executable(tetris_no_gui functional/tetris_no_gui.cpp)
add_executable(tetris_gui_integration functional/tetris_gui_integration.cpp)

add_executable(unit_tests unit/main.cpp
        unit/tetris.cpp
        unit/tetromino_and_generator.cpp)

include_directories(project/include)

target_link_libraries(test_sfml sfml-graphics)
target_link_libraries(tetris_gui_integration gui-lib)

target_link_libraries(unit_tests Boost::unit_test_framework)
target_link_libraries(unit_tests tetris-lib)

target_link_libraries(tetris_no_gui tetris-lib)
target_link_libraries(tetris_gui_integration tetris-lib ai-lib)

# copy /res folder to a folder containing binary
add_custom_command(TARGET tetris_gui_integration POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:tetris_gui_integration>/res
        )