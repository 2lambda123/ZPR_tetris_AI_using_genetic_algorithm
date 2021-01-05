#ifndef GENETIC_TETRIS_EVOLVE_SCREEN_HPP
#define GENETIC_TETRIS_EVOLVE_SCREEN_HPP

#include <AI/evolutionary_strategy.hpp>
#include <gui/gui_utils.hpp>

#include "screen.hpp"

namespace gentetris {

class EvolveScreen : public Screen {
public:
    EvolveScreen(sf::RenderWindow& window, EvolutionaryStrategy& ai, const Tetris& tetris_ai);
    void update() override;
    void draw() override;
    void reset() override;
    bool pollEvent(sf::Event& event) override;

    void createInfo();
    void createBackButton();
    void createStartStopButton();
    void createSaveButton();

private:
    const Tetris& tetris_ai_;

    TetrisBoard board_ai_;

    sf::Text info_;
    Button start_stop_button_;
    Button back_button_;
    Button save_button_;

    EvolutionaryStrategy& ai_;
};

}

#endif  // GENETIC_TETRIS_EVOLVE_SCREEN_HPP
