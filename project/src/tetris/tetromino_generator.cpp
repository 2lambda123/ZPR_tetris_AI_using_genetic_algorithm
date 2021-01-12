/*
 * Author: Rafal Kulus
 */

#include "tetris/tetromino_generator.hpp"

#include <cstdlib>
#include <deque>
#include <vector>

#include "tetris/tetromino.hpp"

namespace genetic_tetris {

const std::vector<Tetromino>& TetrominoGenerator::getTetrominoes() {
    static const std::vector<Tetromino> TETROMINOES = {
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
                  {{0, 2}, {1, 2}, {1, 1}, {2, 1}})};
    return TETROMINOES;
}

TetrominoGenerator::TetrominoGenerator() { generateTetrominoes(); }

Tetromino TetrominoGenerator::getNextTetromino() {
    Tetromino next_tetromino = queue_.front();
    queue_.pop_front();
    if (queue_.size() < QUEUE_LENGTH) {
        generateTetrominoes();
    }
    return next_tetromino;
}

std::deque<Tetromino> TetrominoGenerator::getQueue() const {
    return std::deque<Tetromino>(queue_.begin(), queue_.begin() + QUEUE_LENGTH);
}

void TetrominoGenerator::generateTetrominoes() {
    while (queue_.size() < QUEUE_LENGTH) {
        std::vector<Tetromino> bag(getTetrominoes());
        while (!bag.empty()) {
            unsigned int item_idx = rand() % bag.size();
            queue_.push_back(bag[item_idx]);
            bag.erase(bag.begin() + item_idx);
        }
    }
}

}  // namespace genetic_tetris