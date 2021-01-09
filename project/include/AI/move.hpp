/*
 * Author: Damian Kolaska
 */

#ifndef GENETIC_TETRIS_MOVE_HPP
#define GENETIC_TETRIS_MOVE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>

#include "tetris/tetris.hpp"

namespace genetic_tetris {

class Move {
public:
    inline static const int MIN_MOVE = -1;
    inline static const int MAX_MOVE = Tetris::GRID_WIDTH - 1;
    inline static const int MIN_ROT = 0;
    inline static const int MAX_ROT = 3;

    Move();
    Move(int moveX, int rotations);

    Move(const Move &other);
    Move &operator=(const Move &other);

    /**
     * Performs the move
     * @param tetris tetris on which move will be applied
     * @param hard_drop if true will perform hard drop and calculate tetris properties
     */
    void apply(Tetris &tetris, bool hard_drop = true);

    int getMoveX() const { return move_x_; }
    int getRotation() const { return rotations_; }

    int getMaxHeight() const { return max_height_; }
    int getCumulativeHeight() const { return cumulative_height_; }
    int getRelativeHeight() const { return relative_height_; }
    int getHoles() const { return holes_; }
    int getRoughness() const { return roughness_; }

private:

    static int calculateHoles(const Tetris::Grid &grid);

    /// Calculates grid properties after Move::apply
    void calculateGridProperties(const Tetris &tetris);

    /// Move in x direction
    int move_x_;
    /// Number of rotations
    int rotations_;

    /// Maximum height
    int max_height_ = 0;
    /// Sum of all column heights
    int cumulative_height_ = 0;
    /// Difference between lowest and highest column
    int relative_height_ = 0;
    /// Number of holes in a grid
    int holes_ = 0;
    /// Sum of height differences of all adjacent columns
    int roughness_ = 0;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_MOVE_HPP
