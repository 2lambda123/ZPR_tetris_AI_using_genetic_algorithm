#include "app.hpp"

#include <iostream>
#include <SFML/Window/Keyboard.hpp>

App::App() : gui(WINDOW_WIDTH, WINDOW_HEIGHT) { tick_interval = sf::seconds(0.5f); }
void App::run() {
    game_clock.restart();
    while (!closed) {
        update();
        display();
    }
}
void App::update() {
    pollEvents();
    if (game_clock.getElapsedTime() > tick_interval) {
        tetris.tick();
        game_clock.restart();
    }
    gui.update(tetris.getGrid());
}
void App::display() { 
    gui.draw();
     }
void App::pollEvents() {
    sf::Event event;
    while (gui.pollEvent(event)) {
        if (event.type == sf::Event::Closed) close();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            tetris.shiftLeft();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            tetris.shiftRight();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            tetris.rotateCW();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            tetris.hardDrop();
        }
    }
}
void App::close() {
    closed = true;
    gui.close();
}