#include "tetris/tetromino_generator.hpp"

#include <cstdlib>
#include <deque>
#include <vector>

#include "tetris/tetromino.hpp"

namespace gentetris {

const std::vector<Tetromino> TetrominoGenerator::TETROMINOES = {
    Tetromino(Tetromino::Color::CYAN, Tetromino::Shape::I, {1.5, 1.5},
              {{0, 2}, {1, 2}, {2, 2}, {3, 2}}),
    Tetromino(Tetromino::Color::BLUE, Tetromino::Shape::J, {1, 1},
              {{0, 2}, {0, 1}, {1, 1}, {2, 1}}),
    Tetromino(Tetromino::Color::ORANGE, Tetromino::Shape::L, {1, 1},
              {{0, 1}, {1, 1}, {2, 1}, {2, 2}}),
    Tetromino(Tetromino::Color::YELLOW, Tetromino::Shape::O, {1.5, 1.5},
              {{1, 2}, {2, 2}, {1, 1}, {2, 1}}),
    Tetromino(Tetromino::Color::GREEN, Tetromino::Shape::S, {1, 1},
              {{0, 1}, {1, 1}, {1, 2}, {2, 2}}),
    Tetromino(Tetromino::Color::PURPLE, Tetromino::Shape::T, {1, 1},
              {{0, 1}, {1, 1}, {2, 1}, {1, 2}}),
    Tetromino(Tetromino::Color::RED, Tetromino::Shape::Z, {1, 1},
              {{0, 2}, {1, 2}, {1, 1}, {2, 1}})
};

TetrominoGenerator::TetrominoGenerator() : last_tetromino_(Tetromino()) { generateTetrominoes(); }

Tetromino TetrominoGenerator::getNextTetromino() {
    last_tetromino_ = queue_.front();
    queue_.pop_front();
    if (queue_.size() < QUEUE_LENGTH) {
        generateTetrominoes();
    }
    return last_tetromino_;
}

Tetromino TetrominoGenerator::getLastTetromino() const { return last_tetromino_; }

std::deque<Tetromino> TetrominoGenerator::getQueue() const {
    return std::deque<Tetromino>(queue_.begin(), queue_.begin() + QUEUE_LENGTH);
}

/**
 * Uses 7-bag Random Generator
 * https://tetris.fandom.com/wiki/Random_Generator
 */
void TetrominoGenerator::generateTetrominoes() {
    while (queue_.size() < QUEUE_LENGTH) {
        std::vector<Tetromino> bag(TETROMINOES);
        while (!bag.empty()) {
            unsigned int item_idx = rand() % bag.size();
            queue_.push_back(bag[item_idx]);
            bag.erase(bag.begin() + item_idx);
        }
    }
}

}  // namespace gentetris