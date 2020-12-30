#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "observer.hpp"
#include "tetris.hpp"
#include "move.hpp"


class AI : public Observer {
public:
    AI(Tetris &tetris) : tetris_(tetris) { std::srand(std::time(nullptr)); }
    virtual ~AI() {}

    virtual void operator()() = 0;
    void finish() { finish_ = true; }
    virtual void drop() = 0;

protected:
    Move best_move_;
    Tetris &tetris_;

    volatile bool finish_ = false;
};

#endif