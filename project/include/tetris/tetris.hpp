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
    using Position = std::pair<int, int>;
    using Grid = std::vector<std::vector<Tetromino::Color> >;

    static const int GRID_WIDTH = 10;
    static const int GRID_VISIBLE_HEIGHT = 20;
    static const int GRID_FULL_HEIGHT = 40;
    static constexpr Position TETROMINO_INITIAL_POS = {(GRID_WIDTH / 2) - 2,
                                                                  GRID_VISIBLE_HEIGHT + 1};

    Tetris();
    bool tick();
    void shiftLeft();
    void shiftRight();
    void hardDrop();
    void softDrop();
    void rotateCW();
    void rotateCCW();
    Grid getGrid() const;
    std::string toString() const;
    bool isFinished() const;
    unsigned int getScore() const { return score_; }

protected:
    bool is_finished_;
    virtual void generateTetromino();

private:
    bool isValidPosition(Position tetromino_position) const;
    Position getHardDropPosition() const;


    TetrominoGenerator generator_;
    Tetromino tetromino_;
    Position tetromino_position_;
    Grid grid_;
    unsigned int score_;
};

class ObservableTetris : public Tetris, public Subject {
public:
    ObservableTetris() : Tetris() {}

private:
    void generateTetromino() override;
};

#endif