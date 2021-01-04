#ifndef APP_HPP
#define APP_HPP

#include <AI/evolutionary_strategy.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AI/ai.hpp"
#include "event_manager.hpp"
#include "gui/gui.hpp"
#include "tetris/tetris.hpp"

namespace gentetris {

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

    const int SOFT_DROP_INTERVAL_ = 3;

    const sf::Time tick_interval_ = sf::seconds(0.5f);
    const sf::Time ai_move_interval_ = sf::seconds(0.1f);

    const std::string BACKGROUND_MUSIC_FILE = "res/tetris_theme.ogg";
    const float BACKGROUND_MUSIC_VOLUME = 25.0f;

    enum class State {
        MENU,
        STARTED,
        CLOSED,
    } state_ = State::MENU;

    void pollSfmlEvents();
    void close();
    void start();
    void reset();

    EventManager& event_manager_;

    GUI gui_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    EvolutionaryStrategy ai_;

    sf::Clock game_clock_;
    sf::Clock ai_clock_;

    std::thread ai_thread_;

    sf::Music background_music;

    unsigned int tick_count_;
};

}
#endif