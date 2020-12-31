#include "app.hpp"

#include <AI/evolutionary_strategy.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>

namespace gentetris {

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_), ai_(std::ref(tetris_ai_)) {
    gui_.addObserver(this);
    tetris_human_.addObserver(&ai_);
}

void App::run() {
    while (state_ != State::CLOSED) {
        update();
        display();
    }
}

void App::update() {
    pollSfmlEvents();
    if (state_ == State::STARTED) {
        if (tetris_human_.isFinished()) {
            if (ai_clock_.getElapsedTime() > ai_move_interval_) {
                ai_.drop();
                ai_clock_.restart();
            }
        }
        if (game_clock_.getElapsedTime() > tick_interval_) {
            tetris_human_.tick();
            game_clock_.restart();
        }
    }
    gui_.update(tetris_human_, tetris_ai_);
}

void App::display() { gui_.draw(); }

void App::pollSfmlEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) close();
        if (state_ == State::STARTED) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                tetris_human_.shiftLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                tetris_human_.shiftRight();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                tetris_human_.rotateCW();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                tetris_human_.hardDrop();
                ai_.drop();
            }
        }
    }
}

void App::close() {
    state_ = State::CLOSED;
    gui_.close();
    ai_.finish();
    ai_thread_.join();
}
void App::update(GenTetrisEvent e){
    if (e == GenTetrisEvent::PLAY_BUTTON_CLICKED) {
        if (state_ == State::MENU)
            start();

        std::cout << "app: play button clicked" << std::endl;
    }
}

void App::start(){
    state_ = State::STARTED;
    game_clock_.restart();
    ai_clock_.restart();
    ai_thread_ = std::thread([this]() { ai_("res/input.json", "res/output.json"); });
}

}
