#include "controller/evolve_controller.hpp"

#include <AI/evolutionary_algo.hpp>
#include <tetris/tetris.hpp>

namespace genetic_tetris {

EvolveController::EvolveController(Tetris& tetris_ai, EvolutionaryAlgo& ai, GUI& gui)
    : Controller(gui), tetris_ai_(tetris_ai), ai_(ai) {}

void EvolveController::update() {
    if (state_ == State::STOP) {
        return;
    }
    if (tetris_ai_.isFinished()) {
        tetris_ai_ = Tetris();
    }
    auto move = EvolutionaryAlgo::generateBestMove(ai_.getBest(), tetris_ai_);
    move.apply(tetris_ai_);
}

void EvolveController::start() {
    ai_thread_ = std::thread([this]() { ai_(EvolutionaryAlgo::Mode::EVOLVE); });
    state_ = State::START;
}

void EvolveController::reset() {
    finish();
    tetris_ai_ = Tetris();
    state_ = State::STOP;
}

void EvolveController::finish() {
    ai_.finish();
    if (ai_thread_.joinable()) {
        ai_thread_.join();
    }
}

void EvolveController::handleCustomEvent(EventType e) {
    if (e == EventType::START_EVOLVE_BUTTON_CLICKED) {
        reset();
        start();
    } else if (e == EventType::SAVE_BUTTON_CLICKED) {
        ai_.save();
    }
}

}  // namespace genetic_tetris