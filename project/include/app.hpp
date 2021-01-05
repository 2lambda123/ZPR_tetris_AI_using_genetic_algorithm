#ifndef APP_HPP
#define APP_HPP

#include <SFML/Audio/Music.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "AI/ai.hpp"
#include "AI/evolutionary_strategy.hpp"
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

    const float DEFAULT_SOFT_DROP_INTERVAL_ = 0.05f;

    const sf::Time ai_move_interval_ = sf::seconds(0.1f);

    const std::string BACKGROUND_MUSIC_FILE = "res/tetris_theme.ogg";
    const float BACKGROUND_MUSIC_VOLUME = 0.0f; // 0.25f

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

    void handlePlayerInput(const sf::Event& event);

    void humanTick(bool is_soft_drop = false);

    EventManager& event_manager_;

    ObservableTetris tetris_human_;
    Tetris tetris_ai_;

    EvolutionaryStrategy ai_;

    GUI gui_;

    sf::Time tick_interval_;
    sf::Time soft_drop_interval_;
    sf::Clock game_clock_;
    sf::Clock ai_clock_;

    std::thread ai_thread_;

    sf::Music background_music;
};

}  // namespace gentetris
#endif