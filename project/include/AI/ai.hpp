#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>

#include "observer.hpp"
#include "tetris.hpp"

class Move {
public:
    Move() {
        rotations_ = std::rand() % 4;
        move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
    }
    Move (const Move& other) {
        move_x_ = other.move_x_;
        rotations_ = other.rotations_;
    }
    Move& operator=(const Move& other) {
        move_x_ = other.move_x_;
        rotations_ = other.rotations_;
        return *this;
    }
    int8_t getMoveX() const { return move_x_; }
    int8_t getRotation() const { return rotations_; }
    void setMoveX(int8_t value) {
        move_x_ = std::clamp(value, MIN_MOVE, MAX_MOVE);
    }
    void setRotation(int8_t value) {
        rotations_ = std::clamp(value, MIN_ROT, MAX_ROT);
    }
    void incrementMoveX() {
        if (move_x_ + 1 > MAX_MOVE)
            move_x_ = MIN_MOVE;
        else
            move_x_++;
    }
    void decrementMoveX() {
        if (move_x_ - 1 < MIN_MOVE)
            move_x_ = MAX_MOVE;
        else
            move_x_--;
    }
    void incrementRotation() {
        if (rotations_ + 1 > MAX_ROT)
            rotations_ = MIN_ROT;
        else
            rotations_++;
    }
    void decrementRotation() {
        if (rotations_ - 1 < MIN_ROT)
            rotations_ = MAX_ROT;
        else
            rotations_--;
    }
private:
    const int8_t MIN_MOVE = -1;
    const int8_t MAX_MOVE = Tetris::GRID_WIDTH - 1;
    const int8_t MIN_ROT = 0;
    const int8_t MAX_ROT = 3;

    int8_t rotations_;  // 0-3 (number of rotations CW)
    int8_t move_x_;               // (-1, Tetris::GRID_WIDTH - 1)
};

class AI : public Observer {
public:
    AI(Tetris& tetris) : tetris_(tetris) { std::srand(std::time(nullptr)); }
    void finish() { finish_ = true; }
    void drop() { drop_ = true; }
    void humanFinished() { human_finished_ = true; }
    virtual void operator()();
    void update() override;

protected:
    void move(Tetris &tetris, const Move &move);

    Move best_move_;
    Tetris& tetris_;

    volatile bool finish_ = false;
    volatile bool drop_ = false;
    volatile bool human_finished_ = false;
};

#endif