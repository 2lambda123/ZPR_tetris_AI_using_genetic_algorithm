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

namespace genetic_tetris {

class AI : public Observer {
public:
    explicit AI(Tetris &tetris)
        : tetris_(tetris), generator_(RandomNumberGenerator::getInstance()) {}
    ~AI() override = default;

    virtual void finish() { finish_ = true; }
    virtual void drop() = 0;

    void resetTetris() { tetris_ = Tetris(true); }

protected:
    Tetris &tetris_;
    RandomNumberGenerator &generator_;

    volatile bool finish_ = false;
};

}  // namespace genetic_tetris

#endif