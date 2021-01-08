#ifndef GENETIC_TETRIS_GAME_SCREEN_HPP
#define GENETIC_TETRIS_GAME_SCREEN_HPP

#include <map>

#include "gui/gui_utils.hpp"
#include "screen.hpp"

namespace genetic_tetris {

class GameScreen : public Screen {
public:
    enum class State { START, STOP };

    GameScreen(sf::RenderWindow& window, const Tetris& tetris_human, const Tetris& tetris_ai);
    void update() override;
    void draw() override;
    void reset() override;
    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType event) override;

    int getPlayingGeneration() const;
    void setAvailableGenerations(int value);

private:
    /// How long GUI status should be displayed
    const sf::Time STATUS_PERSISTENCE_ = sf::seconds(1.0f);

    // Helper functions for creating GUI elements
    void createHumanScore();
    void createAIScore();
    void createHumanLevel();
    void createAILevel();
    void createHumanLevelProgress();
    void createHumanLevelSpeed();
    void createStartRestartButton();
    void createBackButton();
    void createGenerationDialog();
    void createStatus();

    const Tetris& tetris_human_;
    const Tetris& tetris_ai_;

    State state_;

    TetrisBoard board_human_;
    TetrisBoard board_ai_;
    TetrisBoard next_tetromino_panel_;

    sf::Text human_score_;
    sf::Text human_level_;
    sf::Text human_level_progress_;
    sf::Text human_level_speed_;
    sf::Text ai_score_;
    sf::Text ai_level_;

    Button start_restart_button_;
    Button back_button_;

    /// Dialog used to specify number of generation playing against the player
    IncDecDialog generation_number_dialog_;
    sf::Text generation_text_;
    int available_generations_ = 99;

    sf::Text status_;
    sf::Clock status_clock_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GAME_SCREEN_HPP
