/*
 * Author: Rafal Kulus
 */

#ifndef TETROMINO_GENERATOR_HPP
#define TETROMINO_GENERATOR_HPP

#include <deque>
#include <vector>

#include "tetris/tetromino.hpp"

namespace genetic_tetris {

class TetrominoGenerator {
public:
    /// Number of next tetrominoes player can preview.
    static const unsigned int QUEUE_LENGTH = 4;
    /// Returns all types of tetrominoes available in the game.
    static const std::vector<Tetromino>& getTetrominoes();

    TetrominoGenerator();
    Tetromino getNextTetromino();
    std::deque<Tetromino> getQueue() const;

private:
    /// Uses 7-bag Random Generator. https://tetris.fandom.com/wiki/Random_Generator
    void generateTetrominoes();

    std::deque<Tetromino> queue_;
};

}  // namespace genetic_tetris

#endif