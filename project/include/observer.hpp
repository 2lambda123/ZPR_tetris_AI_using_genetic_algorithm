#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

enum class GenTetrisEvent {
    TETROMINO_DROPPED,
    PLAY_BUTTON_CLICKED,
    GAME_STARTED,
};

class Observer {
public:
    virtual void update(GenTetrisEvent e) = 0;
    virtual ~Observer() {}
};

class Subject {
public:
    void addObserver(Observer* o) { obs_.push_back(o); }
    void notifyObservers(GenTetrisEvent e) {
        for (Observer* o : obs_) {
            o->update(e);
        }
    }
    virtual ~Subject() {}

private:
    std::vector<Observer*> obs_;
};

#endif