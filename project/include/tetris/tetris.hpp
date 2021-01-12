/*
 * Author: Rafal Kulus
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

/**
 * Main Tetris class. Game Engine.
 */
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

    /// Cleared one row at a time
    static const int SCORE_SINGLE = 100;
    /// Cleared two rows at a time
    static const int SCORE_DOUBLE = 300;
    /// Cleared three rows at a time
    static const int SCORE_TRIPLE = 500;
    /// Cleared four rows at a time
    static const int SCORE_TETRIS = 800;
    /// Per dropped line
    static const int SCORE_SOFT_DROP = 1;
    /// Per dropped line
    static const int SCORE_HARD_DROP = 2;

    /// disable_drop_scores can be set to true to disable score for soft and hard drops e.g. for AI
    explicit Tetris(bool disable_drop_scores = false);

    /**
     * @param is_soft_drop indicates if the current tick was caused by a soft drop input
     * @return true if active tetromino has fallen "into place" and a new tetromino has been
     * generated, false otherwise
     */
    bool tick(bool is_soft_drop = false);

    void shiftLeft();
    void shiftRight();
    void hardDrop(bool tick_after_drop = true);
    void rotateCW();
    void rotateCCW();
    /// Returns grid without current tetromino and ghost piece (used in AI calculations)
    Grid getRawGrid() const;
    Grid getDisplayGrid() const;
    /// Returns grid as a string. Used for testing.
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
    /// Checks if specified position is valid for current tetromino
    bool isValidPosition(Position tetromino_position) const;
    Position getHardDropPosition() const;
    void clearLines();
    void addClearedLinesScore();
    void addProgress();
    /// https://tetris.fandom.com/wiki/Tetris_Worlds#Gravity
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