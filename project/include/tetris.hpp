#ifndef TETRIS_HPP
#define TETRIS_HPP

#include "tetromino.hpp"
#include "tetromino_generator.hpp"

#include <string>
#include <utility>
#include <vector>

class Tetris{
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
    TetrominoGenerator generator_;
    Tetromino tetromino_;
    std::pair<int, int> tetrominoPosition_;
    std::vector<std::vector<Tetromino::Color> > grid_;

    bool isValidPosition() const;
    void generateTetromino();
};

#endif