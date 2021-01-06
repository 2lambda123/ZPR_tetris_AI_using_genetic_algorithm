#ifndef GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
#define GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP

#include <AI/evolutionary_strategy.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"

namespace gentetris {

class EvolveController : public Controller {
public:
    EvolveController(Tetris& tetrisAi, EvolutionaryStrategy& ai);
    void update() override;
    void handleSfmlEvent([[maybe_unused]] const sf::Event& e) override {}
    void handleCustomEvent(EventType e) override;
    void start() override;
    void reset() override;
    void finish() override;

private:
    Tetris& tetris_ai_;
    EvolutionaryStrategy& ai_;

    std::thread ai_thread_;
};

}  // namespace gentetris

#endif  // GENETIC_TETRIS_EVOLVE_CONTROLLER_HPP
