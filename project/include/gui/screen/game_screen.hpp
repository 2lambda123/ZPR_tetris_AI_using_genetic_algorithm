#ifndef GENETIC_TETRIS_GAME_SCREEN_HPP
#define GENETIC_TETRIS_GAME_SCREEN_HPP

#include <map>

#include "gui/gui_utils.hpp"
#include "screen.hpp"

namespace gentetris {

class GameScreen : public Screen {
public:
    GameScreen(sf::RenderWindow& window, const Tetris& tetris_human, const Tetris& tetris_ai);
    void update() override;
    void draw() override;
    void reset() override;
    bool pollEvent(sf::Event& event) override;

private:
    void createHumanScore();
    void createAIScore();
    void createHumanLevel();
    void createAILevel();
    void createHumanLevelProgress();
    void createHumanLevelSpeed();
    void createRestartButton();
    void createBackButton();

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

    Button restart_button_;
    Button back_button_;
};

}

#endif  // GENETIC_TETRIS_GAME_SCREEN_HPP
