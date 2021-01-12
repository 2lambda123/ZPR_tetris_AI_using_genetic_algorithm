/*
 * Authors: Damian Kolaska, Rafal Kulus
 */

#ifndef GENETIC_TETRIS_GAME_CONTROLLER_HPP
#define GENETIC_TETRIS_GAME_CONTROLLER_HPP

#include <AI/evolutionary_algo.hpp>
#include <SFML/System/Clock.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"
#include "sound_manager.hpp"

namespace genetic_tetris {

/**
 * Game screen controller
 */
class GameController : public Controller, public Observer {
public:
    enum class State { START, STOP } state_ = State::STOP;

    GameController(ObservableTetris& tetris_human, EvolutionaryAlgo& ai, GUI& gui);
    void update(EventType e) override;

    void update() override;
    void start() override;
    void reset() override;
    void finish() override;

    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType e) override;

private:
    /// Interval between next AI moves (when player has finished)
    const sf::Time AI_MOVE_INTERVAL_ = sf::seconds(0.1f);
    /// Delay after human player's hard drop so the lines won't clear instantly
    const sf::Time HARD_DROP_LOCK_DELAY_ = sf::seconds(0.25f);
    /// soft_drop_interval_ is calculated as min(DEFAULT_SOFT_DROP_INTERVAL_, tick_interval_ / 2)
    const float DEFAULT_SOFT_DROP_INTERVAL_ = 0.05f;

    void humanTick(bool is_soft_drop = false);
    void handlePlayerInput(const sf::Event& event);

    ObservableTetris& tetris_human_;
    EvolutionaryAlgo& ai_;

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
