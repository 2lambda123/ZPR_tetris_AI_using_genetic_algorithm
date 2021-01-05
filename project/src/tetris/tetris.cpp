#include "tetris/tetris.hpp"

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include "tetris/tetromino.hpp"
#include "tetris/wall_kicks.hpp"

namespace gentetris {

Tetris::Tetris(bool disable_drop_scores)
    : score_(0),
      level_(1),
      level_progress_(0),
      level_speed_(1),
      drop_scores_disabled_(disable_drop_scores) {
    for (int i = 0; i < GRID_FULL_HEIGHT; ++i) {
        std::vector<Tetromino::Color> grid_line(GRID_WIDTH, Tetromino::Color::EMPTY);
        grid_.push_back(grid_line);
    }
    is_finished_ = false;
    // Explicit call (instead of just 'generateTetromino()'), because linter didn't like calling
    // a virtual function from constructor.
    Tetris::generateTetromino();
    calculateLevelSpeed();
}

/**
 * @param is_soft_drop indicates if the current tick was caused by a soft drop input
 * @return true if active tetromino has fallen "into place" and a new tetromino has been generated,
 * false otherwise
 */
bool Tetris::tick(bool is_soft_drop) {
    if (is_finished_) {
        return false;
    }
    --tetromino_position_.second;
    if (isValidPosition(tetromino_position_)) {
        if (is_soft_drop && !drop_scores_disabled_) {
            score_ += SCORE_SOFT_DROP;
        }
        return false;
    }
    ++tetromino_position_.second;
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = tetromino_position_.first + square.first;
        int y = tetromino_position_.second + square.second;

        //      The condition below should never evaluate to true and is kinda pointless,
        //      but I'm leaving it in case somebody changed GRID_VISIBLE_HEIGHT
        //      and / or GRID_FULL_HEIGHT to some strange values.
        if (y >= GRID_FULL_HEIGHT) {
            is_finished_ = true;
            return false;
        }

        grid_[y][x] = tetromino_.getColor();
    }

    unsigned int cleared_lines = clearLines();
    addClearedLinesScore(cleared_lines);
    addProgress(cleared_lines);
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
    int old_y = tetromino_position_.second;
    tetromino_position_ = getHardDropPosition();
    if (!drop_scores_disabled_) {
        score_ += (old_y - tetromino_position_.second) * SCORE_HARD_DROP;
    }
    tick();
}

void Tetris::rotateCW() { rotate(false); }

void Tetris::rotateCCW() { rotate(true); }

Tetris::Grid Tetris::getRawGrid() const { return grid_; }

Tetris::Grid Tetris::getDisplayGrid() const {
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
    Grid static_grid = getDisplayGrid();
    std::string str;
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

unsigned int Tetris::getScore() const { return score_; }

unsigned int Tetris::getLevel() const { return level_; }

unsigned int Tetris::getLevelProgress() const { return level_progress_; }

double Tetris::getLevelSpeed() const { return level_speed_; }

bool Tetris::isValidPosition(Position tetromino_position) const {
    for (const Tetromino::Square& square : tetromino_.getSquares()) {
        int x = tetromino_position.first + square.first;
        int y = tetromino_position.second + square.second;
        if (x < 0 || x > GRID_WIDTH - 1 || y < 0) {
            return false;
        }
        if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_FULL_HEIGHT &&
            grid_[y][x] != Tetromino::Color::EMPTY) {
            return false;
        }
    }
    return true;
}

Tetris::Position Tetris::getHardDropPosition() const {
    Position drop_pos = tetromino_position_;
    do {
        --drop_pos.second;
    } while (isValidPosition(drop_pos));
    ++drop_pos.second;
    return drop_pos;
}

unsigned int Tetris::clearLines() {
    unsigned int cleared = 0;
    int i = 0;
    while (i < GRID_FULL_HEIGHT) {
        bool is_filled_line = true;
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid_[i][j] == Tetromino::Color::EMPTY) {
                is_filled_line = false;
                break;
            }
        }
        if (is_filled_line) {
            grid_.erase(grid_.begin() + i);
            std::vector<Tetromino::Color> grid_line(GRID_WIDTH, Tetromino::Color::EMPTY);
            grid_.push_back(grid_line);
            ++cleared;
        } else {
            ++i;
        }
    }
    return cleared;
}

void Tetris::addClearedLinesScore(unsigned int cleared_lines) {
    switch (cleared_lines) {
        case 1:
            score_ += SCORE_SINGLE * level_;
            break;
        case 2:
            score_ += SCORE_DOUBLE * level_;
            break;
        case 3:
            score_ += SCORE_TRIPLE * level_;
            break;
        case 4:
            score_ += SCORE_TETRIS * level_;
            break;
        default:
            break;
    }
}

void Tetris::addProgress(unsigned int cleared_lines) {
    level_progress_ += cleared_lines;
    if (level_progress_ >= LINES_PER_LEVEL) {
        // zero or mod? Decided to leave it at zero so progress is slightly slower.
        level_progress_ = 0;
        level_ = level_ < MAX_LEVEL ? level_ + 1 : MAX_LEVEL;
        calculateLevelSpeed();
    }
}

/**
 * https://tetris.fandom.com/wiki/Tetris_Worlds#Gravity
 */
void Tetris::calculateLevelSpeed() {
    double speed = pow(0.8 - ((level_ - 1) * 0.007), level_ - 1);
    level_speed_ = speed;
}

void Tetris::rotate(bool ccw) {
    if (tetromino_.getShape() == Tetromino::Shape::O) {
        return;
    }
    int from = tetromino_.getCurrentRotation();
    if (!ccw) {
        tetromino_.rotateCW();
    } else {
        tetromino_.rotateCCW();
    }
    int to = tetromino_.getCurrentRotation();
    std::vector<Position> wall_kicks;
    if (tetromino_.getShape() == Tetromino::Shape::I) {
        wall_kicks = WallKicks::getITetrominoWallKicks(from, to);
    } else {
        wall_kicks = WallKicks::getGenericWallKicks(from, to);
    }
    if (wall_kicks.empty()) {
        throw std::domain_error("Empty wall kick data!");
    }
    for (const Position& offset : wall_kicks) {
        Position new_pos = {tetromino_position_.first + offset.first,
                            tetromino_position_.second + offset.second};
        if (isValidPosition(new_pos)) {
            tetromino_position_ = new_pos;
            return;
        }
    }
    if (!ccw) {
        tetromino_.rotateCCW();
    } else {
        tetromino_.rotateCW();
    }
}

void Tetris::generateTetromino() {
    tetromino_ = generator_.getNextTetromino();
    tetromino_position_ = TETROMINO_INITIAL_POS;
    if (tetromino_.getShape() == Tetromino::Shape::I) {
        --tetromino_position_.second;
    }
    if (!isValidPosition(tetromino_position_)) {
        is_finished_ = true;
        return;
    }
    if (isValidPosition({tetromino_position_.first, tetromino_position_.second - 1})) {
        --tetromino_position_.second;
    }
}

void ObservableTetris::generateTetromino() {
    Tetris::generateTetromino();
    if (!isFinished()) {  // don't notify at the start of the game
        notifyObservers(
            EventType::TETROMINO_DROPPED);  // notify genetic algorithm that a tetromino has
                                                 // changed (it should then execute hardDrop())
    }
}

}  // namespace gentetris