#ifndef APP_HPP
#define APP_HPP

#include "gui.hpp"
#include "tetris.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

class App {
public:
    App();
    void run();
    void update();
    void display();
private:
    void pollEvents();
    void close();

    bool closed = false;
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 900;

    GUI gui;
    Tetris tetris;
    sf::Clock game_clock;
    sf::Time tick_interval;
};

#endif