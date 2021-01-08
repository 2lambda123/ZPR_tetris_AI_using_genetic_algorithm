#ifndef APP_HPP
#define APP_HPP

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <controller/evolve_controller.hpp>
#include <controller/game_controller.hpp>
#include <controller/menu_controller.hpp>

#include "AI/ai.hpp"
#include "AI/evolutionary_algo.hpp"
#include "event_manager.hpp"
#include "gui/gui.hpp"
#include "sound_manager.hpp"
#include "tetris/tetris.hpp"

namespace genetic_tetris {

/**
 * Main class managing the application.
 * It aggregates all the data and controls current state.
 * Logic and displaying information is pushed to corresponding controllers.
 */
class App {
public:
    App();
    void run();
    void update();
    void display();

private:
    enum class State {
        MENU,
        PLAYING,
        EVOLVING,
        CLOSED,
    };

    const static int WINDOW_WIDTH_ = 800;
    const static int WINDOW_HEIGHT_ = 900;
    const static int FPS_ = 60;

    void pollSfmlEvents();
    void pollCustomEvents();
    void close();
    void start();
    void reset();

    EventManager& event_manager_;
    SoundManager& sound_manager_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    EvolutionaryAlgo ai_;

    GUI gui_;

    GameController game_controller_;
    EvolveController evolve_controller_;
    MenuController menu_controller_;

    State state_;
    Controller* active_controller_;
};

}  // namespace genetic_tetris

#endif