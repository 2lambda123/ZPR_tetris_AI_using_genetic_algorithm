#include "AI/move.hpp"

const int Move::MIN_MOVE = -1;
const int Move::MAX_MOVE = Tetris::GRID_WIDTH - 1;
const int Move::MIN_ROT = 0;
const int Move::MAX_ROT = 3;

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

void Move::apply(Tetris &tetris) {
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
    tetris.hardDrop();
    calculateGridProperties(tetris);
}

void Move::setMoveX(int value) { move_x_ = std::clamp(value, MIN_MOVE, MAX_MOVE); }
void Move::setRotation(int value) { rotations_ = std::clamp(value, MIN_ROT, MAX_ROT); }

void Move::incrementMoveX() {
    if (move_x_ + 1 > MAX_MOVE)
        move_x_ = MIN_MOVE;
    else
        move_x_++;
}

void Move::decrementMoveX() {
    if (move_x_ - 1 < MIN_MOVE)
        move_x_ = MAX_MOVE;
    else
        move_x_--;
}

void Move::incrementRotation() {
    if (rotations_ + 1 > MAX_ROT)
        rotations_ = MIN_ROT;
    else
        rotations_++;
}

void Move::decrementRotation() {
    if (rotations_ - 1 < MIN_ROT)
        rotations_ = MAX_ROT;
    else
        rotations_--;
}

void Move::calculateGridProperties(const Tetris& tetris) {
    auto grid = tetris.getGrid();
    max_height_ = calculateMaxHeight(grid);
    cumulative_height_ = calculateCumulativeHeight(grid);
    holes_ = calculateHoles(grid);
}

int Move::calculateMaxHeight(const Tetris::Grid& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    for (int y = rows - 1; y >= 0; y--) {
        for (int x = 0; x < cols; x++) {
            if (grid[y][x] != Tetromino::EMPTY) {
                return y + 1;
            }
        }
    }
    return 0;
}

int Move::calculateCumulativeHeight(const Tetris::Grid &grid) {
    int cumulative_height = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int x = 0; x < cols; x++) {
        for (int y = rows - 1; y >= 0; y--) {
            if (grid[y][x] != Tetromino::EMPTY) {
                cumulative_height += y + 1;
                break;
            }
        }
    }
    return cumulative_height;
}

int Move::calculateHoles(const Tetris::Grid& grid) {
    int holes = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int y = rows - 2; y >= 0; y--) {
        for (int x = 0; x < cols; x++) {
            if (grid[y][x] == Tetromino::EMPTY && grid[y+1][x] != Tetromino::EMPTY) {
                holes++;
            }
        }
    }
    return holes;
}
