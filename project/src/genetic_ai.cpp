#include "genetic_ai.hpp"

void GeneticAI::operator()() {
    while (!finish_) {
        drop_ = false;
        while (!drop_) {
            best_move_ = Move();
            if (!human_finished_) {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            else {
                drop_ = true;
            }
        }
        move();
        tetris_.hardDrop();
        if (tetris_.isFinished()) {
            finish_ = true;
        }
    }
}

void GeneticAI::update() {
    drop_ = true;
}

void GeneticAI::move() {
    for (int i = 0; i < best_move_.rotations_; ++i) {
        tetris_.rotateCW();
    }
    std::cout << "move_x: " << best_move_.move_x_ << std::endl;
    int tip_x = Tetris::TETROMINO_INITIAL_POS.first;
    int move_x = best_move_.move_x_;
    //int move_x = -1;
    if (move_x > tip_x) {
        for (int i = move_x; i > tip_x; --i) {
            tetris_.shiftRight();
        }
    }
    else if (move_x < tip_x) {
        for (int i = move_x; i < tip_x; ++i) {
            tetris_.shiftLeft();
        }
    }
}