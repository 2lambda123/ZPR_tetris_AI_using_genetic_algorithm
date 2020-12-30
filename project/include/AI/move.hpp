#ifndef GENETIC_TETRIS_MOVE_HPP
#define GENETIC_TETRIS_MOVE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>

#include "tetris/tetris.hpp"

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
    int getHoles() const { return holes_; }

private:
    void calculateTetrisProperties(const Tetris& tetris);

    int calculateMaxHeight(const Tetris::Grid& grid) {
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

    int calculateHoles(const Tetris::Grid& grid) {
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

    int move_x_;
    int rotations_;

    int max_height_;
    int holes_;
};

#endif  // GENETIC_TETRIS_MOVE_HPP
