#include "gui/gui.hpp"

namespace gentetris {

GUI::GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris,
         EvolutionaryStrategy& ai)
    : window_(sf::VideoMode(width, height), "Tetris AI"),
      game_screen_(window_, human_tetris, ai_tetris),
      menu_screen_(window_),
      evolve_screen_(window_, ai, ai_tetris) {
    window_.setFramerateLimit(fps);
}

void GUI::update() { active_screen_->update(); }

void GUI::setActiveScreen(GUI::ScreenType screen_type) {
    switch (screen_type) {
        case ScreenType::GAME:
            active_screen_ = &game_screen_;
            break;
        case ScreenType::MENU:
            active_screen_ = &menu_screen_;
            break;
        case ScreenType::EVOLVE:
            active_screen_ = &evolve_screen_;
            break;
    }
    active_screen_->reset();
}

}  // namespace gentetris
