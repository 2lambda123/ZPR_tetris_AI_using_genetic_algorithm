#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include "move.hpp"
#include "random_number_generator.hpp"
#include "tetris/tetris.hpp"

namespace genetic_tetris {
/**
 * Base class for all algorithms working with genetic_tetris::Tetris
 */
class AI : public Observer {
public:
    explicit AI(Tetris &tetris)
        : tetris_(tetris), generator_(RandomNumberGenerator::getInstance()) {}
    ~AI() override = default;

    /// Tells algorithm to finish
    virtual void finish() { finish_ = true; }
    /// Tells algorithm to make a move
    virtual void drop() = 0;
    /// Resets genetic_tetris::Tetris state
    void resetTetris() { tetris_ = Tetris(); }

protected:
    Tetris &tetris_;
    RandomNumberGenerator &generator_;

    volatile bool finish_ = false;
};

}  // namespace genetic_tetris

#endif