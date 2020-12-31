#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "move.hpp"
#include "observer.hpp"
#include "tetris/tetris.hpp"

class AI : public Observer {
public:
    AI(Tetris &tetris) : tetris_(tetris) { std::srand(std::time(nullptr)); }
    virtual ~AI() {}

    virtual void operator()() = 0;
    virtual void finish() { finish_ = true; }
    virtual void drop() = 0;

protected:
    Tetris &tetris_;

    volatile bool finish_ = false;
};

#endif