#ifndef APP_HPP
#define APP_HPP

#include <AI/evolutionary_strategy.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AI/ai.hpp"
#include "gui/gui.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

class App : public Observer {
public:
    App();
    void run();
    void update(GenTetrisEvent e) override;
    void update();
    void display();

private:
    const int WINDOW_WIDTH_ = 800;
    const int WINDOW_HEIGHT_ = 900;

    const sf::Time tick_interval_ = sf::seconds(0.5f);
    const sf::Time ai_move_interval_ = sf::seconds(0.1f);

    enum class State {
        MENU,
        STARTED,
        CLOSED,
    };

    void pollSfmlEvents();
    void close();
    //void reset();
    void start();

    State state_ = State::MENU;

    GUI gui_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    EvolutionaryStrategy ai_;

    sf::Clock game_clock_;
    sf::Clock ai_clock_;

    std::thread ai_thread_;
};

}
#endif