#include "app.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>
#include <AI/evolutionary_strategy.hpp>

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_) {
    ai_ = std::make_unique<EvolutionaryStrategy>(std::ref(tetris_ai_));
    tetris_human_.add(ai_.get());
}

void App::run() {
    game_clock_.restart();
    ai_clock_.restart();
    std::thread ai_thread(std::ref(*ai_.get()));
    while (!closed_) {
        update();
        display();
    }
    ai_->drop();
    ai_->finish();
    ai_thread.join();
}

void App::update() {
    pollEvents();
    if (tetris_human_.isFinished()) {
        if (ai_clock_.getElapsedTime() > ai_move_interval_) {
            ai_->drop();
            ai_clock_.restart();
        }
    }
    if (game_clock_.getElapsedTime() > tick_interval_) {
        tetris_human_.tick();
        game_clock_.restart();
    }
    gui_.update(tetris_human_, tetris_ai_);
}

void App::display() { gui_.draw(); }

void App::pollEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) close();

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
            ai_->drop();
        }
    }
}

void App::close() {
    closed_ = true;
    gui_.close();
}