#ifndef GENETIC_TETRIS_MOVE_HPP
#define GENETIC_TETRIS_MOVE_HPP

#include <algorithm>
#include <chrono>
#include <iostream>

#include "tetris.hpp"

class Move {
public:
    static const int8_t MIN_MOVE;
    static const int8_t MAX_MOVE;
    static const int8_t MIN_ROT;
    static const int8_t MAX_ROT;

    Move() {
        rotations_ = std::rand() % 4;
        move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
    }
    Move(int8_t moveX, int8_t rotations);

    Move(const Move &other);
    Move &operator=(const Move &other);

    void apply(Tetris &tetris);

    int8_t getMoveX() const { return move_x_; }
    int8_t getRotation() const { return rotations_; }
    void setMoveX(int8_t value);
    void setRotation(int8_t value);
    void incrementMoveX();
    void decrementMoveX();
    void incrementRotation();
    void decrementRotation();

private:
    void calculateTetrisProperties();

    int8_t move_x_;
    int8_t rotations_;

    uint8_t max_height;
    uint16_t holes;
};

#endif  // GENETIC_TETRIS_MOVE_HPP
