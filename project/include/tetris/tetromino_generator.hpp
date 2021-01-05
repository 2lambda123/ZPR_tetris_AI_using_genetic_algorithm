#ifndef TETROMINO_GENERATOR_HPP
#define TETROMINO_GENERATOR_HPP

#include <deque>
#include <vector>

#include "tetris/tetromino.hpp"

namespace gentetris {

class TetrominoGenerator {
public:
    static const unsigned int QUEUE_LENGTH = 4;
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

}  // namespace gentetris

#endif