/*
 * Author: Damian Kolaska
 */

#ifndef GENETIC_TETRIS_MENU_CONTROLLER_HPP
#define GENETIC_TETRIS_MENU_CONTROLLER_HPP

#include "controller.hpp"

namespace genetic_tetris {

/**
 * Menu screen controller.
 * It is a dummy class used so we don't have to make
 * additional checks for active_controller_ pointer
 */
class MenuController : public Controller {
public:
    explicit MenuController(GUI& gui) : Controller(gui) {}

    void update() override {}
    void start() override {}
    void reset() override {}
    void finish() override {}

    void handleSfmlEvent(const sf::Event&) override {}
    void handleCustomEvent(EventType) override {}
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_MENU_CONTROLLER_HPP
