#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

#include "event_manager.hpp"

namespace genetic_tetris {

class Observer {
public:
    virtual ~Observer() {}

    virtual void update(EventType e) = 0;
};

class Subject {
public:
    virtual ~Subject() {}

    void addObserver(Observer* o) { obs_.push_back(o); }

    void notifyObservers(EventType e) {
        for (Observer* o : obs_) {
            o->update(e);
        }
    }

private:
    std::vector<Observer*> obs_;
};

}  // namespace genetic_tetris

#endif