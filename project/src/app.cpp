#include "app.hpp"

#include <iostream>

App::App() : gui(WINDOW_WIDTH, WINDOW_HEIGHT) { tick_interval = sf::seconds(0.01f); }
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
        // std::cout << tetris.toString() << std::endl;
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
    }
}
void App::close() {
    closed = true;
    gui.close();
}