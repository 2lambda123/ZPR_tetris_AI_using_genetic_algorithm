#include "tetris/tetris.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "tetris/tetromino.hpp"
#include "tetris/tetromino_generator.hpp"

Tetris::Tetris() : score_(0) {
    for (int i = 0; i < GRID_VISIBLE_HEIGHT; ++i) {
        std::vector<Tetromino::Color> grid_line(GRID_WIDTH, Tetromino::Color::EMPTY);
        grid_.push_back(grid_line);
    }
    is_finished_ = true;

    generateTetromino();  // is_finished_ needs to be true before this statement otherwise notifying
                          // on tetromino change will break
    is_finished_ = false;
}

bool Tetris::tick() {
    if (is_finished_) {
        return false;
    }
    --tetromino_position_.second;
    if (isValidPosition(tetromino_position_)) {
        return false;
    }
    ++tetromino_position_.second;
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;
        // That's not how the game is supposed to end, but the whole project is in a proof of
        // concept stage and it should work as of now. At least we should no longer see segmentation
        // fault.
        if (y >= GRID_VISIBLE_HEIGHT) {
            is_finished_ = true;
            return false;
        }
        grid_[y][x] = tetromino_.getColor();
    }
    int i = 0;
    while (i < GRID_VISIBLE_HEIGHT) {
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
    if (!isValidPosition(tetromino_position_)) {
        ++tetromino_position_.first;
    }
}

void Tetris::shiftRight() {
    ++tetromino_position_.first;
    if (!isValidPosition(tetromino_position_)) {
        --tetromino_position_.first;
    }
}

void Tetris::hardDrop() {
    while (!is_finished_ && !tick())
        ;
}

void Tetris::softDrop() {

}

void Tetris::rotateCW() {
    tetromino_.rotateCW();
    while (!isValidPosition(tetromino_position_)) {
        tetromino_.rotateCW();
    }
}

void Tetris::rotateCCW() {
    tetromino_.rotateCCW();
    while (!isValidPosition(tetromino_position_)) {
        tetromino_.rotateCCW();
    }
}

Tetris::Grid Tetris::getGrid() const {
    Grid static_grid;
    for (int i = 0; i < GRID_VISIBLE_HEIGHT; ++i) {
        std::vector<Tetromino::Color> line(grid_[i]);
        static_grid.push_back(line);
    }
    Position ghost_piece_pos = getHardDropPosition();
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = ghost_piece_pos.first + square.first;
        int y = ghost_piece_pos.second + square.second;
        if (y < GRID_VISIBLE_HEIGHT) {
            static_grid[y][x] = Tetromino::Color::GHOST;
        }
    }
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;
        if (y < GRID_VISIBLE_HEIGHT) {
            static_grid[y][x] = tetromino_.getColor();
        }
    }
    return static_grid;
}

std::string Tetris::toString() const {
    Grid static_grid = getGrid();
    std::string str = "";
    for (int i = GRID_VISIBLE_HEIGHT - 1; i >= 0; --i) {
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

bool Tetris::isFinished() const { return is_finished_; }

bool Tetris::isValidPosition(Position tetromino_position) const {
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = tetromino_position.first + square.first;
        int y = tetromino_position.second + square.second;
        if (x < 0 || x > GRID_WIDTH - 1 || y < 0) {
            return false;
        }
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_VISIBLE_HEIGHT &&
            grid_[y][x] != Tetromino::Color::EMPTY) {
            return false;
        }
    }
    return true;
}

Tetris::Position Tetris::getHardDropPosition() const{
    Position drop_pos = tetromino_position_;
    do{
        --drop_pos.second;
    }while(isValidPosition(drop_pos));
    ++drop_pos.second;
    return drop_pos;
}

void Tetris::generateTetromino() {
    tetromino_ = generator_.getTetromino();
    // tetromino_position_ = std::pair<int, int>((GRID_WIDTH / 2) - 2, GRID_VISIBLE_HEIGHT + 1);
    tetromino_position_ = TETROMINO_INITIAL_POS;
}

void ObservableTetris::generateTetromino() {
    Tetris::generateTetromino();
    if (!is_finished_) {  // don't notify at the start of the game
        notifyObservers(
            GenTetrisEvent::TETROMINO_DROPPED);  // notify genetic algorithm that a tetromino has changed (it should then execute
                   // hardDrop())
    }
}
