#ifndef GENETIC_TETRIS_GAME_SCREEN_HPP
#define GENETIC_TETRIS_GAME_SCREEN_HPP

#include <map>

#include "gui/gui_utils.hpp"
#include "screen.hpp"

namespace genetic_tetris {

class GameScreen : public Screen {
public:
    enum class State {
        START,
        STOP
    } state_ = State::STOP;

    GameScreen(sf::RenderWindow& window, const Tetris& tetris_human, const Tetris& tetris_ai);
    void update() override;
    void draw() override;
    void reset() override;
    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType event) override;
    int getNumberGenerations() const { return generation_number_dialog_.getValue(); }

private:
    const sf::Time STATUS_PERSISTANCE = sf::seconds(1.0f);

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

    IncDecDialog generation_number_dialog_;
    sf::Text generation_text_;

    sf::Text status_;
    sf::Clock status_clock_;
};

}

#endif  // GENETIC_TETRIS_GAME_SCREEN_HPP
