#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include "ai_utils.hpp"
#include "move.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

class AI : public Observer {
public:
    AI(Tetris &tetris) : tetris_(tetris) {}
    virtual ~AI() {}

    virtual void finish() { finish_ = true; }
    virtual void drop() = 0;

protected:
    Tetris &tetris_;

    volatile bool finish_ = false;

    inline static RandomNumberGenerator &generator_ = RandomNumberGenerator::getInstance();
};

}

#endif