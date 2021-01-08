#ifndef GENETIC_TETRIS_EVOLVE_SCREEN_HPP
#define GENETIC_TETRIS_EVOLVE_SCREEN_HPP

#include <AI/evolutionary_algo.hpp>
#include <gui/gui_utils.hpp>

#include "screen.hpp"

namespace genetic_tetris {

class EvolveScreen : public Screen {
public:
    EvolveScreen(sf::RenderWindow& window, EvolutionaryAlgo& ai, const Tetris& tetris_ai);

    void update() override;
    void draw() override;
    void reset() override;
    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType event) override;

    // Helper functions for creating GUI elements
    void createInfo();
    void createBackButton();
    void createStartStopButton();
    void createSaveButton();
    void createStatus();

private:
    /// How long GUI status should be displayed
    const sf::Time STATUS_PERSISTENCE_ = sf::seconds(1.0f);

    EvolutionaryAlgo& ai_;
    const Tetris& tetris_ai_;

    TetrisBoard board_ai_;

    /// Evolutionary algorithm info
    sf::Text info_;
    /// GUI status
    sf::Text status_;
    Button start_stop_button_;
    Button back_button_;
    Button save_button_;

    sf::Clock status_clock_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVOLVE_SCREEN_HPP
