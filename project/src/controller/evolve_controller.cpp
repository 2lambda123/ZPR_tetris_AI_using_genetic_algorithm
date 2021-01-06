#include "controller/evolve_controller.hpp"

#include <AI/evolutionary_strategy.hpp>
#include <tetris/tetris.hpp>

namespace gentetris {

EvolveController::EvolveController(Tetris& tetrisAi, EvolutionaryStrategy& ai)
    : tetris_ai_(tetrisAi), ai_(ai) {}

void EvolveController::update() {
    if (tetris_ai_.isFinished()) {
        tetris_ai_ = Tetris();
    }
    auto move = EvolutionaryStrategy::generateBestMove(ai_.getBest(), tetris_ai_);
    move.apply(tetris_ai_);
}

void EvolveController::handleCustomEvent(EventType e) {
    if (e == EventType::START_STOP_BUTTON_CLICKED) {
        reset();
        start();
    } else if (e == EventType::SAVE_BUTTON_CLICKED) {
        ai_.save();
    }
}

void EvolveController::start() {
    ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::EVOLVE); });
}

void EvolveController::reset() {
    finish();
    tetris_ai_ = Tetris(true);
}

void EvolveController::finish() {
    ai_.finish();
    if (ai_thread_.joinable()) {
        ai_thread_.join();
    }
}

}  // namespace gentetris