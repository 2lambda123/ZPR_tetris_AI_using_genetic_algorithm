#ifndef GENETIC_TETRIS_EVENT_MANAGER_HPP
#define GENETIC_TETRIS_EVENT_MANAGER_HPP

#include <list>

#include "utils.hpp"

namespace gentetris {

enum class EventType {
    TETROMINO_DROPPED,
    PLAY_BUTTON_CLICKED,
    RESTART_BUTTON_CLICKED,
    EVOLVE_BUTTON_CLICKED,
    EXIT_BUTTON_CLICKED,
    BACK_BUTTON_CLICKED,
};

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }
    EventType pollEvent() {
        EventType e = events.front();
        events.pop_front();
        return e;
    }
    void addEvent(const EventType& e) {
        events.push_back(e);
    }
    bool isEmpty() const { return events.empty(); }

private:
    EventManager() {}
    EventManager(const EventManager&) = delete;
    EventManager operator=(const EventManager&) = delete;

    std::list<EventType> events;
};

}

#endif  // GENETIC_TETRIS_EVENT_MANAGER_HPP
