#ifndef TETROMINO_GENERATOR_HPP
#define TETROMINO_GENERATOR_HPP

#include <vector>
#include <deque>

#include "tetris/tetromino.hpp"

class TetrominoGenerator {
public:
    static const unsigned int QUEUE_LENGTH = 3;
    static const std::vector<Tetromino> TETROMINOES;

    TetrominoGenerator();
    Tetromino getNextTetromino();
    Tetromino getLastTetromino() const;
    std::deque<Tetromino> getQueue() const;

private:
    void generateTetrominoes();

    std::deque<Tetromino> queue_;
    Tetromino last_tetromino_;
};

#endif