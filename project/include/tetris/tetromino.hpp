/*
 * Author: Rafal Kulus
 */

#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <string>
#include <utility>
#include <vector>

namespace genetic_tetris {

/**
 * Class representing active (playable) tetromino.
 */
class Tetromino {
public:
    using Pivot = std::pair<double, double>;
    using Square = std::pair<int, int>;
    using Squares = std::vector<Square>;
    using Rotations = std::vector<Squares>;

    enum class Color { EMPTY, CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE, GHOST };
    enum class Shape { NO_SHAPE, I, O, T, S, Z, J, L };

    static Squares rotate(const Squares& squares, const Pivot& pivot, double rad);

    Tetromino();
    Tetromino(Color color, Shape shape, Pivot pivot, const Squares& squares);
    Tetromino(const Tetromino& tetromino) = default;
    Tetromino& operator=(const Tetromino& tetromino) = default;
    void rotateCW();
    void rotateCCW();
    Color getColor() const;
    Shape getShape() const;
    const Squares& getSquares() const;
    int getCurrentRotation() const;

private:
    Color color_;
    Shape shape_;
    Rotations rotations_;
    int current_rotation_;
};

}  // namespace genetic_tetris

#endif