#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "gui/utils.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

class GUI : public Observer, public Subject {
public:
    void update(GenTetrisEvent e) override;
    GUI(int width, int height, int fps);
    void update(const Tetris& tetris_human, const Tetris& tetris_ai);
    void draw();
    void close() { window_.close(); }
    bool pollEvent(sf::Event& event);
    void reset() {
        board_human_.reset();
        board_ai_.reset();
    }

private:
    const sf::Color BG_COLOR = sf::Color(207, 185, 151);
    const std::string FONT_FILE = "res/HackBoldNerdFontCompleteMono.ttf";
    const int FONT_SIZE = 24;

    sf::RenderWindow window_;

    TetrisBoard board_human_;
    TetrisBoard board_ai_;
    TetrisBoard next_tetromino_panel_;

    Button play_button_;

    sf::Font font_;

    sf::Text human_score_;
    sf::Text human_level_;
    sf::Text human_level_progress_;
    sf::Text human_level_speed_;
    sf::Text ai_score_;
};

}  // namespace gentetris

#endif