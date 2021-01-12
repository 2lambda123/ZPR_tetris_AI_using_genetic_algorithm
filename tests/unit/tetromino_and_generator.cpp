/*
 * Author: Rafal Kulus
 */

#include <boost/test/unit_test.hpp>
#include <deque>
#include <iostream>
#include <set>
#include <vector>

#include "tetris/tetromino.hpp"
#include "tetris/tetromino_generator.hpp"

using namespace genetic_tetris;

BOOST_AUTO_TEST_SUITE(tetromino_and_generator)

BOOST_AUTO_TEST_CASE(generator_is_7_bag) {
    std::cout << "Test: Generator is using 7-bag generator...\n";
    TetrominoGenerator gen;
    const int rounds = 10;
    const int num = 7;
    for (int i = 0; i < rounds; ++i) {
        std::set<Tetromino::Shape> tetrominoes;
        for (const Tetromino& tetromino : TetrominoGenerator::getTetrominoes()) {
            tetrominoes.insert(tetromino.getShape());
        }
        BOOST_REQUIRE(tetrominoes.size() == num);
        for (int j = 0; j < num; ++j) {
            Tetromino t = gen.getNextTetromino();
            auto found = tetrominoes.find(t.getShape());
            BOOST_REQUIRE(found != tetrominoes.end());
            if (found != tetrominoes.end()) {
                tetrominoes.erase(found);
            }
        }
        BOOST_REQUIRE(tetrominoes.empty());
    }
}

BOOST_AUTO_TEST_CASE(tetromino_generator_queue) {
    std::cout << "Test: Tetromino generator queue works as expected...\n";
    TetrominoGenerator gen;
    const int rounds = 100;
    std::deque<Tetromino> queue = gen.getQueue();
    for (int i = 0; i < rounds; ++i) {
        Tetromino tetromino = gen.getNextTetromino();
        BOOST_REQUIRE(tetromino.getShape() == queue.front().getShape());
        queue.pop_front();
        std::deque<Tetromino> new_queue = gen.getQueue();
        BOOST_REQUIRE(new_queue.size() == TetrominoGenerator::QUEUE_LENGTH);
        BOOST_REQUIRE(queue.size() == new_queue.size() - 1);
        for (unsigned int j = 0; j < queue.size(); ++j) {
            BOOST_REQUIRE(queue[j].getShape() == new_queue[j].getShape());
        }
        queue = new_queue;
    }
}

BOOST_AUTO_TEST_CASE(tetromino_rotation_and_size) {
    std::cout << "Test: All tetrominoes in all positions fit in a 4x4 box and rotate by 360deg...\n";
    for (Tetromino tetromino : TetrominoGenerator::getTetrominoes()) {
        Tetromino original(tetromino);
        for (int i = 0; i < 4; ++i) {
            for (const Tetromino::Square& square : tetromino.getSquares()) {
                BOOST_REQUIRE(square.first >= 0 && square.first < 4);
                BOOST_REQUIRE(square.second >= 0 && square.second < 4);
            }
            BOOST_REQUIRE(tetromino.getCurrentRotation() == i % 4);
            tetromino.rotateCW();
        }
        BOOST_REQUIRE(tetromino.getCurrentRotation() == 0);
        BOOST_REQUIRE(original.getSquares() == tetromino.getSquares());
    }
    BOOST_REQUIRE(TetrominoGenerator::getTetrominoes().size() == 7);
}

BOOST_AUTO_TEST_SUITE_END()