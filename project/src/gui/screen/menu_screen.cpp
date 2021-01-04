#include "gui/screen/menu_screen.hpp"

#include <event_manager.hpp>

namespace gentetris {

gentetris::MenuScreen::MenuScreen(sf::RenderWindow& window) : Screen(window) {
    createPlayButton();
    createEvolveButton();
    createExitButton();
}

void gentetris::MenuScreen::update() {
    play_button_.update();
    evolve_button_.update();
    exit_button_.update();
}

void gentetris::MenuScreen::draw() {
    window_.clear(BG_COLOR);
    window_.draw(play_button_);
    window_.draw(evolve_button_);
    window_.draw(exit_button_);
    window_.display();
}

void gentetris::MenuScreen::reset() {}
bool gentetris::MenuScreen::pollEvent(sf::Event& event) {
    bool event_polled = window_.pollEvent(event);
    if (event_polled) {
        play_button_.handleEvent(event, window_);
        evolve_button_.handleEvent(event, window_);
        exit_button_.handleEvent(event, window_);
        return true;
    }
    return false;
}

void gentetris::MenuScreen::createPlayButton() {
    play_button_.setPosition(sf::Vector2f(300, 600));
    play_button_.setSize(sf::Vector2f(200, 50));
    play_button_.setText("PLAY", font_);
    play_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(GenTetrisEvent::PLAY_BUTTON_CLICKED); });
}

void gentetris::MenuScreen::createEvolveButton() {
    evolve_button_.setPosition(sf::Vector2f(300, 660));
    evolve_button_.setSize(sf::Vector2f(200, 50));
    evolve_button_.setText("EVOLVE", font_);
    evolve_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(GenTetrisEvent::EVOLVE_BUTTON_CLICKED); });
}

void gentetris::MenuScreen::createExitButton() {
    exit_button_.setPosition(sf::Vector2f(300, 720));
    exit_button_.setSize(sf::Vector2f(200, 50));
    exit_button_.setText("EXIT", font_);
    exit_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(GenTetrisEvent::EXIT_BUTTON_CLICKED); });
}

}