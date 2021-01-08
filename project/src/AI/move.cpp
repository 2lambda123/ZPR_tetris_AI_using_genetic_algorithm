#include "AI/move.hpp"

namespace genetic_tetris {

Move::Move() {
    rotations_ = std::rand() % 4;
    move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
}

Move::Move(int moveX, int rotations) : move_x_(moveX), rotations_(rotations) {}

Move::Move(const Move &other) {
    move_x_ = other.move_x_;
    rotations_ = other.rotations_;
}

Move &Move::operator=(const Move &other) {
    move_x_ = other.move_x_;
    rotations_ = other.rotations_;
    return *this;
}

/**
 * @param tetris
 * @param hard_drop defaults to true, but needs to be false when in PvAI mode for a smooth drop.
 * When false, doesn't invoke calculateGridProperties(),
 * but this function call doesn't have any effect when playing a normal (PvAI) game.
 */
void Move::apply(Tetris &tetris, bool hard_drop) {
    for (int i = 0; i < getRotation(); ++i) {
        tetris.rotateCW();
    }
    int tip_x = Tetris::TETROMINO_INITIAL_POS.first;
    int move_x = getMoveX();
    if (move_x > tip_x) {
        for (int i = move_x; i > tip_x; --i) {
            tetris.shiftRight();
        }
    } else if (move_x < tip_x) {
        for (int i = move_x; i < tip_x; ++i) {
            tetris.shiftLeft();
        }
    }
    if (hard_drop) {
        tetris.hardDrop(true);
        calculateGridProperties(tetris);
    }
}

int Move::calculateHoles(const Tetris::Grid &grid) {
    int holes = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int y = rows - 2; y >= 0; y--) {
        for (int x = 0; x < cols; x++) {
            if (grid[y][x] == Tetromino::Color::EMPTY &&
                grid[y + 1][x] != Tetromino::Color::EMPTY) {
                holes++;
            }
        }
    }
    return holes;
}

void Move::calculateGridProperties(const Tetris &tetris) {
    auto grid = tetris.getRawGrid();
    holes_ = calculateHoles(grid);
    max_height_ = cumulative_height_ = roughness_ = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    int min_height = rows;
    int prev_height, cur_height;
    for (int x = 0; x < cols; x++) {
        cur_height = 0;
        for (int y = rows - 1; y >= 0; y--) {
            if (grid[y][x] != Tetromino::Color::EMPTY) {
                cumulative_height_ += y + 1;
                cur_height = y + 1;
                if (y + 1 > max_height_) {
                    max_height_ = y + 1;
                }
                break;
            } else if (y < min_height) {
                min_height = y;
            }
        }
        if (x > 0) {
            roughness_ += std::abs(cur_height - prev_height);
        }
        prev_height = cur_height;
    }
    relative_height_ = max_height_ - min_height;
}

}  // namespace genetic_tetris
