#ifndef GENETIC_AI_HPP
#define GENETIC_AI_HPP

#include "tetris.hpp"

#include <chrono>
#include <thread>
#include <iostream>

class GeneticAI {
public:
    GeneticAI(): finish_(false) {
        std::srand(std::time(nullptr));
    }
    void finish() { finish_ = true; }
    void operator()(Tetris& tetris) {
        while (!finish_) {
            int move = std::rand() % 3;
            switch (move)
            {
            case 0: 
                tetris.shiftLeft();
                break;
            case 1:
                tetris.shiftRight();
                break;
            case 2:
                tetris.rotateCCW();
                break;
            default:
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
private:
    volatile bool finish_;
};

#endif