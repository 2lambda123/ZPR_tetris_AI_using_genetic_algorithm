#ifndef GENETIC_TETRIS_EVENT_MANAGER_HPP
#define GENETIC_TETRIS_EVENT_MANAGER_HPP

#include <list>

#include "utils.hpp"

namespace genetic_tetris {

enum class EventType {
    TETROMINO_DROPPED,
    PLAY_BUTTON_CLICKED,
    START_GAME_BUTTON_CLICKED,
    EVOLVE_BUTTON_CLICKED,
    EXIT_BUTTON_CLICKED,
    BACK_BUTTON_CLICKED,
    SAVE_BUTTON_CLICKED,
    START_EVOLVE_BUTTON_CLICKED,
    GENOMES_SAVED,
    GENERATION_OUT_OF_BOUNDS,
    GAME_STARTED,
    GAME_START_FAILED,
    GENERATIONS_UPDATED,
};

class EventManager {
public:
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }

    EventManager(const EventManager&) = delete;
    EventManager operator=(const EventManager&) = delete;

    EventType pollEvent() {
        EventType e = events.front();
        events.pop_front();
        return e;
    }

    void addEvent(const EventType& e) { events.push_back(e); }
    bool isEmpty() const { return events.empty(); }
    void removeEvent(EventType event) { events.remove(event); }

private:
    EventManager() = default;

    std::list<EventType> events;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_EVENT_MANAGER_HPP
