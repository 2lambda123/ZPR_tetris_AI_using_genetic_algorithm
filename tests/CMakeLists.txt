add_executable(test_catch2 test_catch2.cpp)
add_executable(test_boost test_boost.cpp)
add_executable(test_sfml test_sfml.cpp)
add_executable(test_tetromino test_tetromino.cpp)

target_link_libraries(test_sfml sfml-graphics)

if (UNIX)
    target_link_libraries(test_boost ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
elseif (WIN32)
    target_link_libraries(test_boost Boost::unit_test_framework)
endif()

target_link_libraries(test_tetromino tetris-lib)