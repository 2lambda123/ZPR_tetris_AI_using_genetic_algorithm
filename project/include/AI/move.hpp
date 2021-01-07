#ifndef GENETIC_TETRIS_MOVE_HPP
#define GENETIC_TETRIS_MOVE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>

#include "tetris/tetris.hpp"

namespace genetic_tetris {

class Move {
public:
    // TODO: why the hell do I need 'inline'? (everywhere else works without it) + warning
    inline static const int MIN_MOVE = -1;
    inline static const int MAX_MOVE = Tetris::GRID_WIDTH - 1;
    inline static const int MIN_ROT = 0;
    inline static const int MAX_ROT = 3;

    Move();
    Move(int moveX, int rotations);

    Move(const Move &other);
    Move &operator=(const Move &other);

    void apply(Tetris &tetris, bool hard_drop = true);

    int getMoveX() const { return move_x_; }
    int getRotation() const { return rotations_; }
    void setMoveX(int value);
    void setRotation(int value);
    void incrementMoveX();
    void decrementMoveX();
    void incrementRotation();
    void decrementRotation();

    int getMaxHeight() const { return max_height_; }
    int getCumulativeHeight() const { return cumulative_height_; }
    int getRelativeHeight() const { return relative_height_; }
    int getHoles() const { return holes_; }
    int getRoughness() const { return roughness_; }

private:
    static int calculateHoles(const Tetris::Grid &grid);

    void calculateGridProperties(const Tetris &tetris);

    int move_x_;
    int rotations_;

    int max_height_ = 0;
    int cumulative_height_ = 0;
    int relative_height_ = 0;
    int holes_ = 0;
    int roughness_ = 0;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_MOVE_HPP
