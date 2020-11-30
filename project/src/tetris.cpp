#include "tetris.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "tetromino.hpp"
#include "tetromino_generator.hpp"

using Square = std::pair<int, int>;
using Squares = std::vector<Square>;

Tetris::Tetris() {
    is_finished_ = false;
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        std::vector<Tetromino::Color> grid_line(GRID_WIDTH, Tetromino::Color::EMPTY);
        grid_.push_back(grid_line);
    }
    generateTetromino();
}

bool Tetris::tick() {
    if (is_finished_) {
        return false;
    }
    --tetromino_position_.second;
    if (isValidPosition()) {
        return false;
    }
    ++tetromino_position_.second;
    for (const Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;
        // That's not how the game is supposed to end, but the whole project is in a proof of
        // concept stage and it should work as of now. At least we should no longer see segmentation
        // fault.
        if (y >= GRID_HEIGHT) {
            is_finished_ = true;
            return false;
        }
        grid_[y][x] = tetromino_.getColor();
    }
    int i = 0;
    while (i < GRID_HEIGHT) {
        bool is_filled_line = true;
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid_[i][j] == Tetromino::Color::EMPTY) {
                is_filled_line = false;
            }
        }
        if (is_filled_line) {
            grid_.erase(grid_.begin() + i);
            std::vector<Tetromino::Color> grid_line(GRID_WIDTH, Tetromino::Color::EMPTY);
            grid_.push_back(grid_line);
        } else {
            ++i;
        }
    }
    generateTetromino();
    return true;
}

void Tetris::shiftLeft() {
    --tetromino_position_.first;
    if (!isValidPosition()) {
        ++tetromino_position_.first;
    }
}

void Tetris::shiftRight() {
    ++tetromino_position_.first;
    if (!isValidPosition()) {
        --tetromino_position_.first;
    }
}

void Tetris::hardDrop() {
    while (!is_finished_ && !tick());
}

void Tetris::rotateCW() {
    tetromino_.rotateCW();
    while (!isValidPosition()) {
        tetromino_.rotateCW();
    }
}

void Tetris::rotateCCW() {
    tetromino_.rotateCCW();
    while (!isValidPosition()) {
        tetromino_.rotateCCW();
    }
}

const TetrisGrid Tetris::getGrid() const {
    TetrisGrid static_grid;
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        std::vector<Tetromino::Color> line(grid_[i]);
        static_grid.push_back(line);
    }
    for (const Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;
        if (y < GRID_HEIGHT) {
            static_grid[y][x] = tetromino_.getColor();
        }
    }
    return static_grid;
}

std::string Tetris::toString() const {
    TetrisGrid static_grid = getGrid();
    std::string str = "";
    for (int i = GRID_HEIGHT - 1; i >= 0; --i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (static_grid[i][j] == Tetromino::Color::EMPTY) {
                str += "_";
            } else {
                str += "#";
            }
        }
        str += "\n";
    }
    return str;
}

bool Tetris::isValidPosition() const {
    for (const Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;
        if (x < 0 || x > GRID_WIDTH - 1 || y < 0) {
            return false;
        }
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT &&
            grid_[y][x] != Tetromino::Color::EMPTY) {
            return false;
        }
    }
    return true;
}

void Tetris::generateTetromino() {
    tetromino_ = generator_.getTetromino();
    tetromino_position_ = std::pair((GRID_WIDTH / 2) - 2, GRID_HEIGHT + 1);
}