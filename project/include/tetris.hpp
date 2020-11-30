#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <string>
#include <utility>
#include <vector>

#include "tetromino.hpp"
#include "tetromino_generator.hpp"

using TetrisGrid = std::vector<std::vector<Tetromino::Color> >;

class Tetris {
public:
    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 20;

    Tetris();
    bool tick();
    void shiftLeft();
    void shiftRight();
    void hardDrop();
    void rotateCW();
    void rotateCCW();
    const std::vector<std::vector<Tetromino::Color> > getGrid() const;
    std::string toString() const;

private:
    bool isFinished_;
    TetrominoGenerator generator_;
    Tetromino tetromino_;
    std::pair<int, int> tetrominoPosition_;
    std::vector<std::vector<Tetromino::Color> > grid_;

    bool isValidPosition() const;
    void generateTetromino();
};

#endif