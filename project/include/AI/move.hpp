#ifndef GENETIC_TETRIS_MOVE_HPP
#define GENETIC_TETRIS_MOVE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>

#include "tetris/tetris.hpp"

namespace gentetris {

class Move {
public:
    static const int MIN_MOVE;
    static const int MAX_MOVE;
    static const int MIN_ROT;
    static const int MAX_ROT;

    Move() {
        rotations_ = std::rand() % 4;
        move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
    }
    Move(int moveX, int rotations);

    Move(const Move &other);
    Move &operator=(const Move &other);

    void apply(Tetris &tetris);

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
    void calculateGridProperties(const Tetris &tetris);
    int calculateHoles(const Tetris::Grid &grid);

    int move_x_;
    int rotations_;

    int max_height_;
    int cumulative_height_;
    int relative_height_;
    int holes_;
    int roughness_;
};

}

#endif  // GENETIC_TETRIS_MOVE_HPP
