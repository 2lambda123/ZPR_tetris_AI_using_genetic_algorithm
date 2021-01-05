#ifndef GENETIC_TETRIS_MENU_CONTROLLER_HPP
#define GENETIC_TETRIS_MENU_CONTROLLER_HPP

#include "controller.hpp"
namespace gentetris {
class MenuController : public Controller {
public:
    void update() override {}
    void handleSfmlEvent(const sf::Event& e) override {}
    void handleCustomEvent(EventType e) override {}
    void start() override {}
    void reset() override {}
    void finish() override {}
};
}

#endif  // GENETIC_TETRIS_MENU_CONTROLLER_HPP
