#include "tetromino_generator.hpp"

#include <cstdlib>
#include <utility>
#include <vector>

const std::vector<Tetromino> TetrominoGenerator::tetrominoes = {
    Tetromino(Tetromino::Color::CYAN, Tetromino::Shape::I, std::pair(1.5, 1.5),
              {std::pair(0, 2), std::pair(1, 2), std::pair(2, 2), std::pair(3, 2)}),
    Tetromino(Tetromino::Color::YELLOW, Tetromino::Shape::O, std::pair(1.5, 1.5),
              {std::pair(1, 2), std::pair(2, 2), std::pair(1, 1), std::pair(2, 1)}),
    Tetromino(Tetromino::Color::PURPLE, Tetromino::Shape::T, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(2, 1), std::pair(1, 2)}),
    Tetromino(Tetromino::Color::GREEN, Tetromino::Shape::S, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(1, 2), std::pair(2, 2)}),
    Tetromino(Tetromino::Color::RED, Tetromino::Shape::Z, std::pair(1, 1),
              {std::pair(0, 2), std::pair(1, 2), std::pair(1, 1), std::pair(2, 1)}),
    Tetromino(Tetromino::Color::BLUE, Tetromino::Shape::J, std::pair(1, 1),
              {std::pair(0, 2), std::pair(0, 1), std::pair(1, 1), std::pair(2, 1)}),
    Tetromino(Tetromino::Color::ORANGE, Tetromino::Shape::L, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(2, 1), std::pair(2, 2)})};

Tetromino TetrominoGenerator::getTetromino() const {
    int random = std::rand() % tetrominoes.size();
    return Tetromino(tetrominoes[random]);
}