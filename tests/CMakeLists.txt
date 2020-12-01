add_executable(test_boost test_boost.cpp)
add_executable(test_sfml test_sfml.cpp)
add_executable(tetris_no_gui tetris_no_gui.cpp)
add_executable(tetris_gui_integration tetris_gui_integration.cpp)
add_executable(main main.cpp)

include_directories(project/include)

target_link_libraries(test_sfml sfml-graphics)
target_link_libraries(tetris_gui_integration sfml-graphics)
target_link_libraries(tetris_gui_integration gui-lib)

if (UNIX)
    target_link_libraries(test_boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    target_link_libraries(main ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
elseif (WIN32)
    target_link_libraries(test_boost Boost::unit_test_framework)
    target_link_libraries(main Boost::unit_test_framework)
endif()

target_link_libraries(tetris_no_gui tetris-lib)
target_link_libraries(tetris_gui_integration tetris-lib)

target_link_libraries(main tetris-lib)