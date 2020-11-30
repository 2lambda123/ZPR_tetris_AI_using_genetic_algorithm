#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <string>
#include <utility>
#include <vector>

#include "tetromino.hpp"
#include "tetromino_generator.hpp"

class Tetris {
public:
    using Grid = std::vector<std::vector<Tetromino::Color> >;

    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 20;

    Tetris();
    bool tick();
    void shiftLeft();
    void shiftRight();
    void hardDrop();
    void rotateCW();
    void rotateCCW();
    const Grid getGrid() const;
    std::string toString() const;

private:
    bool is_finished_;
    TetrominoGenerator generator_;
    Tetromino tetromino_;
    std::pair<int, int> tetromino_position_;
    Grid grid_;

    bool isValidPosition() const;
    void generateTetromino();
};

#endif