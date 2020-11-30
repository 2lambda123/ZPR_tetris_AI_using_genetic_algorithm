#include "app.hpp"

#include <iostream>
#include <SFML/Window/Keyboard.hpp>

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_) { tick_interval_ = sf::seconds(0.5f); }

void App::run() {
    game_clock_.restart();
    while (!closed_) {
        update();
        display();
    }
}

void App::update() {
    pollEvents();
    if (game_clock_.getElapsedTime() > tick_interval_) {
        tetris_.tick();
        game_clock_.restart();
    }
    gui_.update(tetris_.getGrid());
}

void App::display() { 
    gui_.draw();
}

void App::pollEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) close();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            tetris_.shiftLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            tetris_.shiftRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            tetris_.rotateCW();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            tetris_.hardDrop();
        }
    }
}

void App::close() {
    closed_ = true;
    gui_.close();
}