#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <string>
#include <utility>
#include <vector>

#include "observer.hpp"
#include "tetromino.hpp"
#include "tetromino_generator.hpp"

class Tetris {
public:
    using Grid = std::vector<std::vector<Tetromino::Color> >;

    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 20;
    static constexpr std::pair<int, int> TETROMINO_INITIAL_POS = {(GRID_WIDTH / 2) - 2,
                                                                  GRID_HEIGHT + 1};

    Tetris();
    bool tick();
    void shiftLeft();
    void shiftRight();
    void hardDrop();
    void rotateCW();
    void rotateCCW();
    const Grid getGrid() const;
    std::string toString() const;
    bool isFinished() const;
    unsigned int getScore() const { return score_; }

private:
    TetrominoGenerator generator_;
    Tetromino tetromino_;
    std::pair<int, int> tetromino_position_;
    Grid grid_;

    unsigned int score_ = 0;

    bool isValidPosition() const;

protected:
    bool is_finished_ = true;
    virtual void generateTetromino();
};

class ObservableTetris : public Tetris, public Subject {
public:
    ObservableTetris() : Tetris() {}

private:
    void generateTetromino() override;
};

#endif