#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

class Observer {
public:
    virtual void update() = 0;
    virtual ~Observer() {}
};

class Subject {
public:
    void add(Observer* o) { obs_.push_back(o); }
    void notify() {
        for (Observer* o : obs_) {
            o->update();
        }
    }
    virtual ~Subject() {}
private:
    std::vector<Observer*> obs_;    
};

#endif