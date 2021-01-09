/*
 * Author: Damian Kolaska
 */

#include "gui/gui.hpp"

namespace genetic_tetris {

GUI::GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris,
         EvolutionaryAlgo& ai)
    : window_(sf::VideoMode(width, height), "Tetris AI"),
      menu_screen_(window_),
      game_screen_(window_, human_tetris, ai_tetris),
      evolve_screen_(window_, ai, ai_tetris),
      active_screen_(&menu_screen_) {
    window_.setFramerateLimit(fps);
}

void GUI::update() { active_screen_->update(); }

void GUI::draw() { active_screen_->draw(); }

void GUI::close() { window_.close(); }

bool GUI::pollEvent(sf::Event& event) { return window_.pollEvent(event); }

void GUI::handleSfmlEvent(const sf::Event& event) { active_screen_->handleSfmlEvent(event); }

void GUI::handleCustomEvent(EventType event) { active_screen_->handleCustomEvent(event); }

void GUI::reset() { active_screen_->reset(); }

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
        default:
            break;
    }
    active_screen_->reset();
}

Screen* GUI::getActiveScreen() { return active_screen_; }

}  // namespace genetic_tetris
