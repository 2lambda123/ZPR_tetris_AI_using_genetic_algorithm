#include "app.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>
#include <AI/classic_genetic_algo.hpp>

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_) {
    ai_ = std::make_unique<ClassicGeneticAlgo>(std::ref(tetris_ai_));
    tick_interval_ = sf::seconds(0.5f);
    tetris_human_.add(ai_.get());
}

void App::run() {
    game_clock_.restart();
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
    if (game_clock_.getElapsedTime() > tick_interval_) {
        tetris_human_.tick();
        if (tetris_human_.isFinished()) {
            ai_->humanFinished();
        }
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