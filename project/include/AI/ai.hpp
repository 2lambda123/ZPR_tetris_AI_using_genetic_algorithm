#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <chrono>
#include <iostream>
#include <thread>

#include "observer.hpp"
#include "tetris.hpp"

class AI : public Observer {
public:
    struct Move {
        Move() {
            rotations_ = std::rand() % 4;
            move_x_ = std::rand() % (Tetris::GRID_WIDTH + 1) - 1;
        }
        unsigned short rotations_;  // 0-3 (number of rotations CW)
        short move_x_;               // (-1, Tetris::GRID_WIDTH - 1)
    };

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