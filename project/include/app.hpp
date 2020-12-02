#ifndef APP_HPP
#define APP_HPP

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "gui.hpp"
#include "tetris.hpp"
#include "genetic_ai.hpp"

class App {
public:
    App();
    void run();
    void update();
    void display();

private:
    void pollEvents();
    void close();

    bool closed_ = false;
    const int WINDOW_WIDTH_ = 800;
    const int WINDOW_HEIGHT_ = 900;

    GUI gui_;
    Tetris tetris_human_;
    Tetris tetris_ai_;
    sf::Clock game_clock_;
    sf::Time tick_interval_;
};

#endif