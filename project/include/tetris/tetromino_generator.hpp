/*
 * Author: Rafał Kulus
 */

#ifndef TETROMINO_GENERATOR_HPP
#define TETROMINO_GENERATOR_HPP

#include <deque>
#include <vector>

#include "tetris/tetromino.hpp"

namespace genetic_tetris {

class TetrominoGenerator {
public:
    static const unsigned int QUEUE_LENGTH = 4;
    static const std::vector<Tetromino>& getTetrominoes();

    TetrominoGenerator();
    Tetromino getNextTetromino();
    std::deque<Tetromino> getQueue() const;

private:
    void generateTetrominoes();

    std::deque<Tetromino> queue_;
};

}  // namespace genetic_tetris

#endif