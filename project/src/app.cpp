#include "app.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>

#include "event_manager.hpp"

namespace gentetris {

App::App()
    : event_manager_(EventManager::getInstance()),
      tetris_human_(),
      tetris_ai_(true),
      ai_(std::ref(tetris_ai_)),
      gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_, FPS_, tetris_human_, tetris_ai_, ai_) {
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    soft_drop_interval_ = sf::seconds(DEFAULT_SOFT_DROP_INTERVAL_);
    if (!background_music.openFromFile(BACKGROUND_MUSIC_FILE)) {
        throw std::runtime_error("Cannot open " + BACKGROUND_MUSIC_FILE);
    }
    background_music.setLoop(true);
    background_music.setVolume(BACKGROUND_MUSIC_VOLUME);
}

void App::run() {
    background_music.play();
    gui_.setActiveScreen(GUI::ScreenType::MENU);
    while (state_ != State::CLOSED) {
        update();
        display();
    }
}

void App::update() {
    pollSfmlEvents();
    pollCustomEvents();
    if (state_ == State::PLAYING) {
        if (tetris_human_.isFinished()) {
            if (ai_clock_.getElapsedTime() > ai_move_interval_) {
                ai_.drop();
                ai_clock_.restart();
            }
        }
        if (game_clock_.getElapsedTime() > tick_interval_) {
            humanTick();
        }
    }
    else if (state_ == State::EVOLVING) {
        if (tetris_ai_.isFinished()) {
            tetris_ai_ = Tetris();
        }
        if (game_clock_.getElapsedTime() > tick_interval_) {
            auto move = EvolutionaryStrategy::generateBestMove(ai_.getBest(), tetris_ai_);
            move.apply(tetris_ai_);
        }
    }
    gui_.update();
}

void App::display() { gui_.draw(); }

void App::pollSfmlEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        }
        if (state_ == State::PLAYING && !tetris_human_.isFinished()) {
            handlePlayerInput(event);
        }
    }
    if (state_ == State::PLAYING && !tetris_human_.isFinished()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
            if (game_clock_.getElapsedTime() > soft_drop_interval_) {
                humanTick(true);
            }
        }
    }
    if (tetris_human_.isFinished() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        ai_.drop();
    }
}

void App::handlePlayerInput(const sf::Event& event) {
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

void App::pollCustomEvents() {
    if (!event_manager_.isEmpty()) {
        EventType e = event_manager_.pollEvent();
        if (e == EventType::PLAY_BUTTON_CLICKED || e == EventType::RESTART_BUTTON_CLICKED) {
            if (state_ == State::MENU) {
                gui_.setActiveScreen(GUI::ScreenType::GAME);
                state_ = State::PLAYING;
            }
            reset();
            start();
        } else if (e == EventType::BACK_BUTTON_CLICKED) {
            if (state_ == State::PLAYING || state_ == State::EVOLVING) {
                gui_.setActiveScreen(GUI::ScreenType::MENU);
                ai_.finish();
                if (ai_thread_.joinable()) {
                    ai_thread_.join();
                }
                state_ = State::MENU;
            }
        } else if (e == EventType::EVOLVE_BUTTON_CLICKED) {
            gui_.setActiveScreen(GUI::ScreenType::EVOLVE);
            state_ = State::EVOLVING;
            reset();
            start();
        } else if (e == EventType::EXIT_BUTTON_CLICKED) {
            close();
        }
    }
}

void App::close() {
    gui_.close();
    if (state_ == State::PLAYING || state_ == State::EVOLVING) {
        ai_.finish();
        if (ai_thread_.joinable()) {
            ai_thread_.join();
        }
    }
    state_ = State::CLOSED;
}

void App::start() {
    game_clock_.restart();
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    soft_drop_interval_ = sf::seconds(DEFAULT_SOFT_DROP_INTERVAL_);
    ai_clock_.restart();
    if (state_ == State::PLAYING) {
        ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::PLAY); });
    } else if (state_ == State::EVOLVING) {
        ai_thread_ = std::thread([this]() { ai_(EvolutionaryStrategy::Mode::EVOLVE); });
    }
}

void App::reset() {
    ai_.finish();
    if (ai_thread_.joinable()) {
        ai_thread_.join();
    }
    tetris_human_ = ObservableTetris();
    tetris_human_.addObserver(&ai_);
    tetris_ai_ = Tetris(true);
    gui_.reset();
}

void App::humanTick(bool is_soft_drop) {
    tetris_human_.tick(is_soft_drop);
    game_clock_.restart();
    tick_interval_ = sf::seconds((float)tetris_human_.getLevelSpeed());
    if (soft_drop_interval_.asSeconds() * 2 > tick_interval_.asSeconds()) {
        soft_drop_interval_ = sf::seconds(tick_interval_.asSeconds() / 2);
    }
}

}  // namespace gentetris
