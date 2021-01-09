/*
 * Author: Damian Kolaska
 */

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

namespace genetic_tetris {

/**
 * Manages screen switching. Displaying is handled by Screen classes.
 */
class GUI {
public:
    enum class ScreenType {
        MENU,
        GAME,
        EVOLVE,
    };

    GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris,
        EvolutionaryAlgo& ai);
    void update();
    void draw();
    void close();
    bool pollEvent(sf::Event& event);
    void handleSfmlEvent(const sf::Event& event);
    void handleCustomEvent(EventType event);
    void reset();
    void setActiveScreen(ScreenType screen_type);
    Screen* getActiveScreen();

private:
    sf::RenderWindow window_;

    MenuScreen menu_screen_;
    GameScreen game_screen_;
    EvolveScreen evolve_screen_;

    Screen* active_screen_;
};

}  // namespace genetic_tetris

#endif