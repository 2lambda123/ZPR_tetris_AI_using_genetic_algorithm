#ifndef GUI_UTILS_HPP
#define GUI_UTILS_HPP

#include <SFML/Graphics.hpp>
#include <gui/screen/evolve_screen.hpp>
#include <gui/screen/game_screen.hpp>
#include <gui/screen/menu_screen.hpp>
#include <gui/screen/screen.hpp>
#include <map>
#include <vector>

#include "gui/gui_utils.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

class GUI {
public:
    enum class ScreenType {
        GAME,
        MENU,
        EVOLVE,
    };

    GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris,
        EvolutionaryStrategy& ai);
    void update();
    void draw() { active_screen_->draw(); }
    void close() { window_.close(); }
    bool pollEvent(sf::Event& event) { return active_screen_->pollEvent(event); }
    void reset() { active_screen_->reset(); }
    void setActiveScreen(ScreenType screen_type);

private:
    sf::RenderWindow window_;

    Screen* active_screen_;
    GameScreen game_screen_;
    MenuScreen menu_screen_;
    EvolveScreen evolve_screen_;
};

}  // namespace gentetris

#endif