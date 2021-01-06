#include "app.hpp"

#include <thread>

#include "event_manager.hpp"
#include "sound_manager.hpp"

namespace gentetris {

App::App()
    : event_manager_(EventManager::getInstance()),
      sound_manager_(SoundManager::getInstance()),
      tetris_human_(),
      tetris_ai_(true),
      ai_(std::ref(tetris_ai_)),
      gui_(WINDOW_WIDTH_, WINDOW_HEIGHT_, FPS_, tetris_human_, tetris_ai_, ai_),
      game_controller_(tetris_human_, ai_, gui_),
      evolve_controller_(tetris_ai_, ai_, gui_),
      menu_controller_(gui_) {}

void App::run() {
    sound_manager_.play(Sound::TETRIS_THEME);
    gui_.setActiveScreen(GUI::ScreenType::MENU);
    active_controller_ = &menu_controller_;
    while (state_ != State::CLOSED) {
        update();
        display();
    }
}

void App::update() {
    pollSfmlEvents();
    pollCustomEvents();
    active_controller_->update();
    gui_.update();
}

void App::display() { gui_.draw(); }

void App::pollSfmlEvents() {
    sf::Event event;
    while (gui_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        }
        active_controller_->handleSfmlEvent(event);
        gui_.handleSfmlEvent(event);
    }
}

void App::pollCustomEvents() {
    if (!event_manager_.isEmpty()) {
        EventType e = event_manager_.pollEvent();
        if (e == EventType::PLAY_BUTTON_CLICKED) {
            if (state_ == State::MENU) {
                gui_.setActiveScreen(GUI::ScreenType::GAME);
                state_ = State::PLAYING;
                active_controller_->finish();
                active_controller_ = &game_controller_;
            }
            reset();
        } else if (e == EventType::BACK_BUTTON_CLICKED) {
            if (state_ == State::PLAYING || state_ == State::EVOLVING) {
                gui_.setActiveScreen(GUI::ScreenType::MENU);
                active_controller_->finish();
                active_controller_ = &menu_controller_;
                state_ = State::MENU;
                reset();
            }
        } else if (e == EventType::EVOLVE_BUTTON_CLICKED) {
            gui_.setActiveScreen(GUI::ScreenType::EVOLVE);
            state_ = State::EVOLVING;
            active_controller_->finish();
            active_controller_ = &evolve_controller_;
            reset();
        } else if (e == EventType::EXIT_BUTTON_CLICKED) {
            close();
        } else {
            active_controller_->handleCustomEvent(e);
            gui_.handleCustomEvent(e);
        }
    }
}

void App::close() {
    gui_.close();
    active_controller_->finish();
    state_ = State::CLOSED;
}

void App::start() { active_controller_->start(); }

void App::reset() {
    active_controller_->reset();
    gui_.reset();
}

}  // namespace gentetris
