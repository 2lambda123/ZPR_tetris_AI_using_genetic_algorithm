#ifndef APP_HPP
#define APP_HPP

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <controller/evolve_controller.hpp>
#include <controller/game_controller.hpp>
#include <controller/menu_controller.hpp>

#include "AI/ai.hpp"
#include "AI/evolutionary_strategy.hpp"
#include "event_manager.hpp"
#include "gui/gui.hpp"
#include "sound_manager.hpp"
#include "tetris/tetris.hpp"

namespace genetic_tetris {

class App {
public:
    App();
    void run();
    void update();
    void display();

private:
    const int WINDOW_WIDTH_ = 800;
    const int WINDOW_HEIGHT_ = 900;
    const int FPS_ = 60;

    enum class State {
        MENU,
        PLAYING,
        EVOLVING,
        CLOSED,
    } state_ = State::MENU;

    void pollSfmlEvents();
    void pollCustomEvents();
    void close();
    void start();
    void reset();

    EventManager& event_manager_;
    SoundManager& sound_manager_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    EvolutionaryStrategy ai_;

    GUI gui_;

    Controller* active_controller_;
    GameController game_controller_;
    EvolveController evolve_controller_;
    MenuController menu_controller_;
};

}  // namespace genetic_tetris

#endif