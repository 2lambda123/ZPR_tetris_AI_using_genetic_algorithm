#ifndef GENETIC_TETRIS_GAME_CONTROLLER_HPP
#define GENETIC_TETRIS_GAME_CONTROLLER_HPP

#include <AI/evolutionary_strategy.hpp>
#include <SFML/System/Clock.hpp>
#include <tetris/tetris.hpp>

#include "controller.hpp"

namespace gentetris {

class GameController : public Controller {
public:
    enum class State {
        START,
        STOP
    } state_ = State::STOP;

    GameController(ObservableTetris& tetris_human, EvolutionaryStrategy& ai, GUI& gui)
        : Controller(gui), tetris_human_(tetris_human), ai_(ai) {
        tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
        soft_drop_interval_ = sf::seconds(DEFAULT_SOFT_DROP_INTERVAL_);
    }

    void update() override {
        if (state_ == State::STOP) return;
        if (ai_.isDroppingSmoothly()) {
            ai_.tick();
            return;
        }
        if (tetris_human_.isFinished()) {
            if (ai_clock_.getElapsedTime() > AI_MOVE_INTERVAL_ ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                ai_.drop();
                ai_clock_.restart();
            }
        } else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
                if (game_clock_.getElapsedTime() > soft_drop_interval_) {
                    humanTick(true);
                }
            }
            if (game_clock_.getElapsedTime() > tick_interval_) {
                humanTick();
            }
        }
    }

    void start() override {
        game_clock_.restart();
        ai_clock_.restart();
        tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
        ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::PLAY); });
        state_ = State::START;
    }

    void finish() override {
        ai_.finish();
        if (ai_thread_.joinable()) {
            ai_thread_.join();
        }
        state_ = State::STOP;
    }

    void reset() override {
        finish();
        tetris_human_ = ObservableTetris();
        tetris_human_.addObserver(&ai_);
        ai_.resetTetris();
    }

    void handleSfmlEvent(const sf::Event& event) override {
        if (!tetris_human_.isFinished() && !ai_.isDroppingSmoothly()) {
            handlePlayerInput(event);
        }
    }

    void handleCustomEvent(EventType e) override {
        if (e == EventType::RESTART_BUTTON_CLICKED) {
            ai_.setGenerationNumber(static_cast<GameScreen*>(gui_.getActiveScreen())->getNumberGenerations());
            EventManager::getInstance().addEvent(EventType::GAME_STARTED);
            reset();
            gui_.reset();
            start();
        }
    }

private:
    const sf::Time AI_MOVE_INTERVAL_ = sf::seconds(0.1f);
    const float DEFAULT_SOFT_DROP_INTERVAL_ = 0.05f;

    void humanTick(bool is_soft_drop = false) {
        tetris_human_.tick(is_soft_drop);
        game_clock_.restart();
        tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
        if (soft_drop_interval_.asSeconds() * 2 > tick_interval_.asSeconds()) {
            soft_drop_interval_ = sf::seconds(tick_interval_.asSeconds() / 2);
        }
    }

    void handlePlayerInput(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::X:
                case sf::Keyboard::Numpad1:
                case sf::Keyboard::Numpad5:
                case sf::Keyboard::Numpad9:
                    tetris_human_.rotateCW();
                    break;
                case sf::Keyboard::LControl:
                case sf::Keyboard::RControl:
                case sf::Keyboard::Z:
                case sf::Keyboard::Numpad3:
                case sf::Keyboard::Numpad7:
                    tetris_human_.rotateCCW();
                    break;
                case sf::Keyboard::Space:
                case sf::Keyboard::Numpad8:
                    tetris_human_.hardDrop();
                    game_clock_.restart();
                    break;
                case sf::Keyboard::Escape:
                case sf::Keyboard::F1:
                    // TODO: pause
                    break;
                case sf::Keyboard::Left:
                case sf::Keyboard::Numpad4:
                    tetris_human_.shiftLeft();
                    break;
                case sf::Keyboard::Right:
                case sf::Keyboard::Numpad6:
                    tetris_human_.shiftRight();
                    break;
                default:
                    break;
            }
        }
    }

    ObservableTetris& tetris_human_;
    EvolutionaryStrategy& ai_;

    sf::Clock ai_clock_;
    sf::Clock game_clock_;
    sf::Time tick_interval_;
    sf::Time soft_drop_interval_;

    std::thread ai_thread_;
};

}  // namespace gentetris

#endif  // GENETIC_TETRIS_GAME_CONTROLLER_HPP
