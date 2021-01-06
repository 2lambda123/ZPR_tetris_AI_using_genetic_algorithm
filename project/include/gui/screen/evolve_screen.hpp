#ifndef GENETIC_TETRIS_EVOLVE_SCREEN_HPP
#define GENETIC_TETRIS_EVOLVE_SCREEN_HPP

#include <AI/evolutionary_strategy.hpp>
#include <gui/gui_utils.hpp>

#include "screen.hpp"

namespace genetic_tetris {

class EvolveScreen : public Screen {
public:
    EvolveScreen(sf::RenderWindow& window, EvolutionaryStrategy& ai, const Tetris& tetris_ai);
    void update() override;
    void draw() override;
    void reset() override;
    void handleSfmlEvent(const sf::Event &event) override;
    void handleCustomEvent(EventType event) override;

    void createInfo();
    void createBackButton();
    void createStartStopButton();
    void createSaveButton();
    void createStatus();

private:
    const sf::Time STATUS_PERSISTANCE = sf::seconds(1.0f);

    EvolutionaryStrategy& ai_;
    const Tetris& tetris_ai_;

    TetrisBoard board_ai_;

    sf::Text info_;
    sf::Text status_;
    Button start_stop_button_;
    Button back_button_;
    Button save_button_;

    sf::Clock status_clock_;
};

}

#endif  // GENETIC_TETRIS_EVOLVE_SCREEN_HPP
