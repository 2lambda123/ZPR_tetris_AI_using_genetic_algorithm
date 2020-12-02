#include "app.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_) { tick_interval_ = sf::seconds(0.5f); }

void App::run() {
    game_clock_.restart();
    GeneticAI genetic_ai;
    std::thread ai_thread(std::ref(genetic_ai), std::ref(tetris_ai_));
    while (!closed_) {
        update();
        display();
    }
}

void App::update() {
    pollEvents();
    if (game_clock_.getElapsedTime() > tick_interval_) {
        tetris_human_.tick();
        tetris_ai_.tick();
        game_clock_.restart();
    }
    gui_.update(tetris_human_.getGrid(), tetris_ai_.getGrid());
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
        }
    }
}

void App::close() {
    closed_ = true;
    gui_.close();
}