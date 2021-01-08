#ifndef GENETIC_TETRIS_CONTROLLER_HPP
#define GENETIC_TETRIS_CONTROLLER_HPP

#include <SFML/Window/Event.hpp>

#include "event_manager.hpp"
#include "gui/gui.hpp"

namespace genetic_tetris {

/**
 * Interface for controller classes.
 * Controllers are used to split logic into
 * more manageable parts.
 */
class Controller {
public:
    explicit Controller(GUI& gui) : gui_(gui) {}

    virtual void update() = 0;
    virtual void start() = 0;
    virtual void reset() = 0;
    virtual void finish() = 0;

    virtual void handleSfmlEvent(const sf::Event& e) = 0;
    virtual void handleCustomEvent(EventType e) = 0;

protected:
    GUI& gui_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_CONTROLLER_HPP
