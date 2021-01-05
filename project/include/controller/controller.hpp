#ifndef GENETIC_TETRIS_CONTROLLER_HPP
#define GENETIC_TETRIS_CONTROLLER_HPP
#include <SFML/Window/Event.hpp>
#include <event_manager.hpp>
namespace gentetris {

class Controller {
public:
    virtual void update() = 0;
    virtual void handleSfmlEvent(const sf::Event& e) = 0;
    virtual void handleCustomEvent(EventType e) = 0;
    virtual void start() = 0;
    virtual void reset() = 0;
    virtual void finish() = 0;
};

}

#endif  // GENETIC_TETRIS_CONTROLLER_HPP
