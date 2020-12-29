#include "AI/ai.hpp"

void AI::operator()() {
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
        move(tetris_, best_move_);
        if (tetris_.isFinished()) {
            finish_ = true;
        }
    }
}

void AI::update() {
    drop_ = true;
}

void AI::move(Tetris &tetris, const Move &move) {
    for (int i = 0; i < best_move_.rotations_; ++i) {
        tetris_.rotateCW();
    }
    int tip_x = Tetris::TETROMINO_INITIAL_POS.first;
    int move_x = best_move_.move_x_;
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
    tetris.hardDrop();
}