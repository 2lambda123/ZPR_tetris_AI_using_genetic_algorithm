#ifndef APP_HPP
#define APP_HPP

#include <AI/evolutionary_strategy.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AI/ai.hpp"
#include "gui.hpp"
#include "tetris/tetris.hpp"

class App {
public:
    App();
    void run();
    void update();
    void display();

private:
    const int WINDOW_WIDTH_ = 800;
    const int WINDOW_HEIGHT_ = 900;

    const sf::Time tick_interval_ = sf::seconds(0.5f);
    const sf::Time ai_move_interval_ = sf::seconds(0.1f);

    void pollEvents();
    void close();

    bool closed_ = false;
    GUI gui_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    //std::unique_ptr<AI> ai_;
    EvolutionaryStrategy ai_;

    sf::Clock game_clock_;
    sf::Clock ai_clock_;
};

#endif