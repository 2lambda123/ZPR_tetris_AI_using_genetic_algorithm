#include "gui/gui.hpp"

#include <cassert>
#include <iostream>

namespace gentetris {

GUI::GUI(int width, int height)
    : window_(sf::VideoMode(width, height), "Tetris AI"),
      board_human_(sf::Vector2f(10, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                   TetrisBoard::TileProperties(30.0f, 0.5f)),
      board_ai_(sf::Vector2f(480, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                TetrisBoard::TileProperties(30.0f, 0.5f)),
      next_tetromino_panel_(sf::Vector2f(337, 10), sf::Vector2i(6, 18),
                            TetrisBoard::TileProperties(20.0f, 0.5f)),
      play_button_(sf::Vector2f(300, 700), sf::Vector2f(200, 50)) {
    if (!font_.loadFromFile(FONT_FILE)) {
        throw std::runtime_error("Error loading font!");
    }
    human_score_.setFont(font_);
    ai_score_.setFont(font_);
    human_score_.setCharacterSize(FONT_SIZE);
    ai_score_.setCharacterSize(FONT_SIZE);
    human_score_.setPosition(sf::Vector2f(10, 640));
    ai_score_.setPosition(sf::Vector2f(480, 640));
    play_button_.setText("PLAY", font_);
    play_button_.addObserver(this);
    addObserver(&play_button_);
}

void GUI::update(const Tetris &tetris_human, const Tetris &tetris_ai) {
    if (tetris_human.isFinished() && !board_human_.isStateFinished())
        board_human_.setStateFinished(true);
    if (tetris_ai.isFinished() && !board_ai_.isStateFinished()) board_ai_.setStateFinished(true);
    board_human_.setState(tetris_human.getGrid());
    board_ai_.setState(tetris_ai.getGrid());
    human_score_.setString("Human: " + std::to_string(tetris_human.getScore()));
    ai_score_.setString("AI: " + std::to_string(tetris_ai.getScore()));
    play_button_.update();
}

void GUI::draw() {
    window_.clear(BG_COLOR);
    board_human_.draw(window_);
    board_ai_.draw(window_);
    next_tetromino_panel_.draw(window_);
    window_.draw(human_score_);
    window_.draw(ai_score_);
    window_.draw(play_button_);
    window_.display();
}

bool GUI::pollEvent(sf::Event& event) {
    bool event_polled = window_.pollEvent(event);
    if (event_polled) {
        play_button_.handleEvent(event, window_);
        return true;
    }
    return false;
}
void GUI::update(GenTetrisEvent e) {
    if (e == GenTetrisEvent::PLAY_BUTTON_CLICKED) {
        notifyObservers(e);
    }
    else if (e == GenTetrisEvent::GAME_STARTED) {
        notifyObservers(e);
    }
}

}  // namespace gentetris
