#ifndef GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
#define GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP

#include <AI/evolutionary_strategy.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"
namespace gentetris {

class EvolveController : public Controller {
public:
    EvolveController(Tetris& tetrisAi, EvolutionaryStrategy& ai) : tetris_ai_(tetrisAi), ai_(ai) {}
    void update() override {
        if (tetris_ai_.isFinished()) {
            tetris_ai_ = Tetris();
        }
        auto move = EvolutionaryStrategy::generateBestMove(ai_.getBest(), tetris_ai_);
        move.apply(tetris_ai_);
    }
    void handleSfmlEvent(const sf::Event& e) override {}
    void handleCustomEvent(EventType e) override {
        if (e == EventType::START_STOP_BUTTON_CLICKED) {
            reset();
            start();
        }
        else if (e == EventType::SAVE_BUTTON_CLICKED) {
            ai_.save();
        }
    }
    void start() override {
        ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::EVOLVE); });
    }
    void reset() override {
        finish();
        tetris_ai_ = Tetris(true);
    }
    void finish() override {
        ai_.finish();
        if (ai_thread_.joinable()) {
            ai_thread_.join();
        }
    }

private:
    Tetris& tetris_ai_;
    EvolutionaryStrategy& ai_;

    std::thread ai_thread_;
};

}

#endif  // GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
