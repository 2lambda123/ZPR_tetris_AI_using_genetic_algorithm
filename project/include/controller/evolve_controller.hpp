#ifndef GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
#define GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP

#include <AI/evolutionary_strategy.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"

namespace genetic_tetris {

class EvolveController : public Controller {
public:
    enum class State { START, STOP } state_ = State::STOP;

    EvolveController(Tetris& tetris_ai, EvolutionaryStrategy& ai, GUI& gui);

    void update() override;
    void start() override;
    void reset() override;
    void finish() override;

    void handleSfmlEvent(const sf::Event&) override {}
    void handleCustomEvent(EventType e) override;

private:
    Tetris& tetris_ai_;
    EvolutionaryStrategy& ai_;

    std::thread ai_thread_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
