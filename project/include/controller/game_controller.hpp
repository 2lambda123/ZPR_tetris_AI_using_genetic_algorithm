#ifndef GENETIC_TETRIS_GAME_CONTROLLER_HPP
#define GENETIC_TETRIS_GAME_CONTROLLER_HPP

#include <AI/evolutionary_strategy.hpp>
#include <SFML/System/Clock.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"
#include "sound_manager.hpp"

namespace genetic_tetris {

class GameController : public Controller, public Observer {
public:
    enum class State { START, STOP } state_ = State::STOP;

    GameController(ObservableTetris& tetris_human, EvolutionaryStrategy& ai, GUI& gui);
    void update(EventType e) override;

    void update() override;
    void start() override;
    void reset() override;
    void finish() override;

    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType e) override;

private:
    const sf::Time AI_MOVE_INTERVAL_ = sf::seconds(0.1f);
    const sf::Time HARD_DROP_LOCK_DELAY_ = sf::seconds(0.25f);
    const float DEFAULT_SOFT_DROP_INTERVAL_ = 0.05f;

    void humanTick(bool is_soft_drop = false);
    void handlePlayerInput(const sf::Event& event);

    ObservableTetris& tetris_human_;
    EvolutionaryStrategy& ai_;

    sf::Clock ai_clock_;
    sf::Clock game_clock_;
    sf::Time tick_interval_;
    sf::Time soft_drop_interval_;

    std::thread ai_thread_;
    SoundManager& sound_manager_;
    bool hard_drop_lock_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GAME_CONTROLLER_HPP
