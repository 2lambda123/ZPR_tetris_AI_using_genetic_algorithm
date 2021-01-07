#include "controller/game_controller.hpp"

#include <AI/evolutionary_strategy.hpp>
#include <SFML/System/Clock.hpp>
#include <tetris/tetris.hpp>

#include "controller/controller.hpp"
#include "sound_manager.hpp"

namespace genetic_tetris {

GameController::GameController(ObservableTetris &tetris_human, EvolutionaryStrategy &ai, GUI &gui)
    : Controller(gui),
      tetris_human_(tetris_human),
      ai_(ai),
      sound_manager_(SoundManager::getInstance()),
      hard_drop_lock_(false) {
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    soft_drop_interval_ = sf::seconds(DEFAULT_SOFT_DROP_INTERVAL_);
}

void GameController::update() {
    if (state_ == State::STOP) {
        return;
    }
    if (ai_.isDroppingSmoothly()) {
        ai_.tick();
        return;
    }
    if (hard_drop_lock_) {
        if (game_clock_.getElapsedTime() > HARD_DROP_LOCK_DELAY_) {
            hard_drop_lock_ = false;
            humanTick();
        }
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

void GameController::start() {
    game_clock_.restart();
    ai_clock_.restart();
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::PLAY); });
    state_ = State::START;
}

void GameController::reset() {
    finish();
    tetris_human_ = ObservableTetris();
    tetris_human_.addObserver(&ai_);
    ai_.resetTetris();
}

void GameController::finish() {
    ai_.finish();
    if (ai_thread_.joinable()) {
        ai_thread_.join();
    }
    state_ = State::STOP;
}

void GameController::handleSfmlEvent(const sf::Event &event) {
    if (state_ == State::START && !tetris_human_.isFinished() && !ai_.isDroppingSmoothly() &&
        !hard_drop_lock_) {
        handlePlayerInput(event);
    }
}

void GameController::handleCustomEvent(EventType e) {
    if (e == EventType::START_GAME_BUTTON_CLICKED) {
        ai_.setGenerationNumber(
            dynamic_cast<GameScreen *>(gui_.getActiveScreen())->getNumberGenerations());
        EventManager::getInstance().addEvent(EventType::GAME_STARTED);
        reset();
        gui_.reset();
        start();
    }
}

void GameController::humanTick(bool is_soft_drop) {
    tetris_human_.tick(is_soft_drop);
    if (tetris_human_.getLastTickClearedRowsCount()) {
        sound_manager_.play(SoundManager::Sound::ROW_CLEARED);
    }
    game_clock_.restart();
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    if (soft_drop_interval_.asSeconds() * 2 > tick_interval_.asSeconds()) {
        soft_drop_interval_ = sf::seconds(tick_interval_.asSeconds() / 2);
    }
}

void GameController::handlePlayerInput(const sf::Event &event) {
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
                tetris_human_.hardDrop(false);
                sound_manager_.play(SoundManager::Sound::HARD_DROP);
                hard_drop_lock_ = true;
                game_clock_.restart();
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

}  // namespace genetic_tetris
