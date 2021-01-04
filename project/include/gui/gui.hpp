#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <gui/screen/game_screen.hpp>
#include <gui/screen/screen.hpp>
#include <map>
#include <vector>

#include "gui/utils.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

class GUI {
public:
    GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris);
    void update(const Tetris& tetris_human, const Tetris& tetris_ai);
    void draw();
    void close() { window_.close(); }
    bool pollEvent(sf::Event& event);
    void reset() {
        game_screen_.reset();
    }

private:

    sf::RenderWindow window_;

    GameScreen game_screen_;
};

}  // namespace gentetris

#endif