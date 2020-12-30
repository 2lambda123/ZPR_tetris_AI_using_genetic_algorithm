#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "observer.hpp"
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
    int8_t move_x_;
    int8_t rotations_;
};

class AI : public Observer {
public:
    AI(Tetris &tetris) : tetris_(tetris) { std::srand(std::time(nullptr)); }

    virtual void operator()() = 0;
    void finish() { finish_ = true; }
    void drop() { drop_ = true; }
    void humanFinished() { human_finished_ = true; }

    void update() override;

protected:
    Move best_move_;
    Tetris &tetris_;

    volatile bool finish_ = false;
    volatile bool drop_ = false;
    volatile bool human_finished_ = false;
};

#endif