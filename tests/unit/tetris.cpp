/*
 * Author: Rafal Kulus
 */

#include <boost/test/unit_test.hpp>
#include <deque>
#include <iostream>
#include <vector>

#include "tetris/tetris.hpp"
#include "tetris/tetromino.hpp"

using namespace genetic_tetris;

BOOST_AUTO_TEST_SUITE(tetris)

BOOST_AUTO_TEST_CASE(tetris_will_finish) {
    std::cout << "Test: Game of Tetris will finish in a finite number of hard drops "
                 "and tetrominoes spawn in correct columns...\n";
    Tetris tetris;
    for (int i = 0; i <= Tetris::GRID_FULL_HEIGHT; ++i) {
        tetris.hardDrop();
    }
    BOOST_REQUIRE(tetris.isFinished());
    unsigned int min_square = Tetris::GRID_WIDTH;
    unsigned int max_square = 0;
    Tetris::Grid grid = tetris.getRawGrid();
    for(unsigned int i = 0; i < grid.size(); ++i){
        for(unsigned int j = 0; j < grid[i].size(); ++j){
            if(grid[i][j] != Tetromino::Color::EMPTY){
                min_square = std::min(min_square, j);
                max_square = std::max(max_square, j);
            }
        }
    }
    BOOST_REQUIRE(min_square >= 3);
    BOOST_REQUIRE(max_square <= 6);
}

BOOST_AUTO_TEST_CASE(tetris_default_config) {
    std::cout << "Test: Game of Tetris starts with correct default config...\n";
    Tetris tetris;
    BOOST_REQUIRE(tetris.getLevel() == 1);
    BOOST_REQUIRE(tetris.getLevelProgress() == 0);
    BOOST_REQUIRE(tetris.getLastTickClearedRowsCount() == 0);
    BOOST_REQUIRE(tetris.getScore() == 0);
    BOOST_REQUIRE(tetris.getLevelSpeed() == 1);
    Tetris::Grid grid = tetris.getRawGrid();
    tetris.hardDrop();
    BOOST_REQUIRE(tetris.getScore() != 0);
    BOOST_REQUIRE(tetris.getRawGrid() != grid);
}

BOOST_AUTO_TEST_SUITE_END()