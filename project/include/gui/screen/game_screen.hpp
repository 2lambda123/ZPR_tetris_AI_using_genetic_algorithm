#ifndef GENETIC_TETRIS_GAME_SCREEN_HPP
#define GENETIC_TETRIS_GAME_SCREEN_HPP

#include "screen.hpp"
#include "gui/utils.hpp"

#include <map>

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
    void createHumanLevelProgress();
    void createHumanLevelSpeed();
    void createHumanLevel();
    void createPlayButton();

    sf::Text createText(const sf::Vector2f& position, int font_size);

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

    Button play_button_;
};

}

#endif  // GENETIC_TETRIS_GAME_SCREEN_HPP
