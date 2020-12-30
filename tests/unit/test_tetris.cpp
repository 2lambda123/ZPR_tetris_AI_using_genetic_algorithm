#ifdef __linux__
#define BOOST_TEST_DYN_LINK
#endif

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <iostream>

#include "tetris/tetris.hpp"
#include "tetris/tetromino.hpp"
#include "tetris/tetromino_generator.hpp"

BOOST_AUTO_TEST_CASE(tetrominoes_exist) {
    std::cout << "Test #1: All tetrominoes exist...\n";
    for (const Tetromino& tetromino : TetrominoGenerator::TETROMINOES) {
        BOOST_REQUIRE(tetromino.getColor() != Tetromino::Color::EMPTY);
        BOOST_REQUIRE(tetromino.getShape() != Tetromino::Shape::NO_SHAPE);
        BOOST_REQUIRE(tetromino.getSquares().size() == 4);
    }
    BOOST_REQUIRE(TetrominoGenerator::TETROMINOES.size() == 7);
    std::cout << "Test #1 passed!\n";
}

BOOST_AUTO_TEST_CASE(tetrominoes_full_rotation) {
    std::cout << "Test #2: All tetrominoes can do a full rotation...\n";
    for (Tetromino tetromino : TetrominoGenerator::TETROMINOES) {
        Tetromino original(tetromino);
        for (int i = 0; i < 4; ++i) {
            tetromino.rotateCW();
        }
        BOOST_REQUIRE(original.getSquares() == tetromino.getSquares());
    }
    std::cout << "Test #2 passed!\n";
}

BOOST_AUTO_TEST_CASE(tetrominoes_squares_placement) {
    std::cout << "Test #3: All tetrominoes in all positions fit in a 4x4 box...\n";
    for (Tetromino tetromino : TetrominoGenerator::TETROMINOES) {
        for (int i = 0; i < 4; ++i) {
            for (const Tetromino::Square& square : tetromino.getSquares()) {
                BOOST_REQUIRE(square.first >= 0 && square.first < 4);
                BOOST_REQUIRE(square.second >= 0 && square.second < 4);
            }
            tetromino.rotateCW();
        }
    }
    BOOST_REQUIRE(TetrominoGenerator::TETROMINOES.size() == 7);
    std::cout << "Test #3 passed!\n";
}

BOOST_AUTO_TEST_CASE(tetris_will_finish) {
    std::cout << "Test #4: Game of Tetris will finish in a finite number of hard drops...\n";
    Tetris tetris;
    for (int i = 0; i <= Tetris::GRID_HEIGHT; ++i) {
        tetris.hardDrop();
    }
    BOOST_REQUIRE(tetris.isFinished());
    std::cout << "Test #4 passed!\n";
}
