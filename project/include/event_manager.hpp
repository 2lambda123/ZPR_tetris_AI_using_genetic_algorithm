#ifndef GENETIC_TETRIS_EVENT_MANAGER_HPP
#define GENETIC_TETRIS_EVENT_MANAGER_HPP

#include <list>

#include "utils.hpp"

namespace gentetris {

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }
    const GenTetrisEvent& peekLastEvent() const {
        return events.front();
    }
    void popLastEvent() {
        events.pop_front();
    }
    void addEvent(const GenTetrisEvent& e) {
        events.push_back(e);
    }
    bool isEmpty() const { return events.empty(); }

private:
    EventManager() {}
    EventManager(const EventManager&) = delete;
    EventManager operator=(const EventManager&) = delete;

    std::list<GenTetrisEvent> events;
};

}

#endif  // GENETIC_TETRIS_EVENT_MANAGER_HPP
