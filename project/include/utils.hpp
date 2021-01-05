#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

#include "event_manager.hpp"

namespace gentetris {


class Observer {
public:
    virtual void update(EventType e) = 0;
    virtual ~Observer() {}
};

class Subject {
public:
    void addObserver(Observer* o) { obs_.push_back(o); }
    void notifyObservers(EventType e) {
        for (Observer* o : obs_) {
            o->update(e);
        }
    }
    virtual ~Subject() {}

private:
    std::vector<Observer*> obs_;
};

}

#endif