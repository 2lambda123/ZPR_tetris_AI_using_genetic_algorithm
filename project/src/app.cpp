#include "app.hpp"

#include <AI/evolutionary_strategy.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <thread>

namespace gentetris {

App::App() : gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_, FPS_), ai_(std::ref(tetris_ai_)), tick_count_(0) {
    gui_.addObserver(this);
    addObserver(&gui_);
    if (!background_music.openFromFile(BACKGROUND_MUSIC_FILE)) {
        throw std::runtime_error("Cannot open " + BACKGROUND_MUSIC_FILE);
    }
    background_music.setLoop(true);
    background_music.setVolume(BACKGROUND_MUSIC_VOLUME);
}

void App::run() {
    background_music.play();
    while (state_ != State::CLOSED) {
        update();
        display();
        ++tick_count_;
    }
}

void App::update() {
    pollSfmlEvents();
    if (state_ == State::STARTED) {
        if (tetris_human_.isFinished()) {
            if (ai_clock_.getElapsedTime() > ai_move_interval_) {
                ai_.drop();
                ai_clock_.restart();
            }
        }
        if (game_clock_.getElapsedTime() > tick_interval_) {
            tetris_human_.tick();
            game_clock_.restart();
        }
    }
    gui_.update(tetris_human_, tetris_ai_);
}

void App::display() { gui_.draw(); }

void App::pollSfmlEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        }
        if (state_ == State::STARTED && !tetris_human_.isFinished()) {
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
    }
    if (state_ == State::STARTED && !tetris_human_.isFinished()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
            if (tick_count_ % SOFT_DROP_INTERVAL_ == 0) {
                tetris_human_.tick(true);
                game_clock_.restart();
            }
        }
    }
    if (tetris_human_.isFinished() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        ai_.drop();
    }
}

void App::close() {
    gui_.close();
    if (state_ == State::STARTED) {
        ai_.finish();
        if (ai_thread_.joinable()) {
            ai_thread_.join();
        }
    }
    state_ = State::CLOSED;
}
void App::update(GenTetrisEvent e) {
    if (e == GenTetrisEvent::PLAY_BUTTON_CLICKED) {
        if (state_ == State::MENU) {
            notifyObservers(GenTetrisEvent::GAME_STARTED);
        }
        reset();
    }
}

void App::start() {
    game_clock_.restart();
    ai_clock_.restart();
    ai_thread_ = std::thread([this]() { ai_("res/input.json", "res/output.json"); });
    state_ = State::STARTED;
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
    start();
}

}  // namespace gentetris
