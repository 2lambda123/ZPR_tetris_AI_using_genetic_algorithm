/*
 * Author: Rafał Kulus
 */

#ifndef TETRIS_HPP
#define TETRIS_HPP

#include <string>
#include <utility>
#include <vector>

#include "tetromino.hpp"
#include "tetromino_generator.hpp"
#include "utils.hpp"

namespace genetic_tetris {

class Tetris {
public:
    using Position = std::pair<int, int>;
    using Grid = std::vector<std::vector<Tetromino::Color> >;

    static const int GRID_WIDTH = 10;
    static const int GRID_VISIBLE_HEIGHT = 20;
    static const int GRID_FULL_HEIGHT = 40;

    static constexpr Position TETROMINO_INITIAL_POS = {(GRID_WIDTH / 2) - 2,
                                                       (GRID_FULL_HEIGHT / 2) - 1};

    static const int MAX_LEVEL = 15;
    static const int LINES_PER_LEVEL = 10;

    static const int SCORE_SINGLE = 100;
    static const int SCORE_DOUBLE = 300;
    static const int SCORE_TRIPLE = 500;
    static const int SCORE_TETRIS = 800;
    static const int SCORE_SOFT_DROP = 1;
    static const int SCORE_HARD_DROP = 2;

    explicit Tetris(bool disable_drop_scores = false);
    bool tick(bool is_soft_drop = false);
    void shiftLeft();
    void shiftRight();
    void hardDrop(bool tick_after_drop = true);
    void rotateCW();
    void rotateCCW();
    Grid getRawGrid() const;
    Grid getDisplayGrid() const;
    std::string toString() const;
    bool isFinished() const;
    unsigned int getScore() const;
    unsigned int getLevel() const;
    unsigned int getLevelProgress() const;
    double getLevelSpeed() const;
    unsigned int getLastTickClearedRowsCount() const;
    std::deque<Tetromino> getTetrominoQueue() const;

protected:
    virtual void generateTetromino();

private:
    bool isValidPosition(Position tetromino_position) const;
    Position getHardDropPosition() const;
    void clearLines();
    void addClearedLinesScore();
    void addProgress();
    void calculateLevelSpeed();
    void rotate(bool ccw);

    TetrominoGenerator generator_;
    Tetromino tetromino_;
    Position tetromino_position_;
    Grid grid_;

    bool is_finished_;

    unsigned int score_;
    unsigned int level_;
    unsigned int level_progress_;
    double level_speed_;

    unsigned int cleared_rows_;

    bool drop_scores_disabled_;
};

class ObservableTetris : public Tetris, public Subject {
public:
    ObservableTetris() : Tetris() {}

private:
    void generateTetromino() override;
};

}  // namespace genetic_tetris

#endif