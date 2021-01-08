#include "gui/screen/evolve_screen.hpp"

#include <AI/evolutionary_algo.hpp>

namespace genetic_tetris {

EvolveScreen::EvolveScreen(sf::RenderWindow& window, EvolutionaryAlgo& ai,
                           const Tetris& tetris_ai)
    : Screen(window),
      ai_(ai),
      tetris_ai_(tetris_ai),
      board_ai_(sf::Vector2f(270, 20),
                sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_VISIBLE_HEIGHT),
                TetrisBoard::TileProperties(25.0f, 0.5f)) {
    createBackButton();
    createStartStopButton();
    createSaveButton();
    createInfo();
    createStatus();
}

void EvolveScreen::update() {
    board_ai_.setState(tetris_ai_.getDisplayGrid());
    back_button_.update();
    start_stop_button_.update();
    save_button_.update();
    info_.setString(ai_.getInfo());
    if (status_clock_.getElapsedTime() > STATUS_PERSISTENCE_) {
        status_.setString("");
    }
}

void EvolveScreen::draw() {
    window_.clear(BG_COLOR);
    board_ai_.draw(window_);
    window_.draw(info_);
    window_.draw(status_);
    window_.draw(back_button_);
    window_.draw(start_stop_button_);
    window_.draw(save_button_);
    window_.display();
}

void EvolveScreen::reset() { board_ai_.reset(); }

void EvolveScreen::handleSfmlEvent(const sf::Event& event) {
    back_button_.handleEvent(event, window_);
    start_stop_button_.handleEvent(event, window_);
    save_button_.handleEvent(event, window_);
}

void EvolveScreen::handleCustomEvent(EventType event) {
    if (event == EventType::GENOMES_SAVED) {
        status_.setString("Genomes saved");
        status_clock_.restart();
    }
}

void EvolveScreen::createInfo() {
    info_ = createText(sf::Vector2f(90, 570), (int)(FONT_SIZE * 0.65));
}

void EvolveScreen::createBackButton() {
    back_button_.setPosition(sf::Vector2f(90, 800));
    back_button_.setSize(sf::Vector2f(200, 50));
    back_button_.setText("BACK", font_);
    back_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(EventType::BACK_BUTTON_CLICKED); });
}

void EvolveScreen::createStartStopButton() {
    start_stop_button_.setPosition(sf::Vector2f(300, 800));
    start_stop_button_.setSize(sf::Vector2f(200, 50));
    start_stop_button_.setText("START", font_);
    start_stop_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(EventType::START_EVOLVE_BUTTON_CLICKED); });
}

void EvolveScreen::createSaveButton() {
    save_button_.setPosition(sf::Vector2f(510, 800));
    save_button_.setSize(sf::Vector2f(200, 50));
    save_button_.setText("SAVE", font_);
    save_button_.setOnClick(
        []() { EventManager::getInstance().addEvent(EventType::SAVE_BUTTON_CLICKED); });
}

void EvolveScreen::createStatus() {
    status_ = createText(sf::Vector2f(10, 870), (int)(FONT_SIZE * 0.65));
}

}  // namespace genetic_tetris
