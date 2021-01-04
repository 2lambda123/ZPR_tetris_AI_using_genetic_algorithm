#include "tetris/tetromino.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>
#include <string>
#include <utility>

Tetromino::Squares Tetromino::rotate(const Squares& squares, const Pivot& pivot, double rad) {
    Squares rotated_squares;
    for (const Square& square : squares) {
        double x = square.first - pivot.first;
        double y = square.second - pivot.second;
        double new_x = x * cos(rad) - y * sin(rad);
        double new_y = y * cos(rad) + x * sin(rad);
        Square new_square = std::make_pair(round(new_x + pivot.first), round(new_y + pivot.second));
        rotated_squares.push_back(new_square);
    }
    return rotated_squares;
}

Tetromino::Tetromino() : color_(Color::EMPTY), shape_(Shape::NO_SHAPE), current_rotation_(0) {}

Tetromino::Tetromino(Color color, Shape shape, Pivot pivot, const Squares& squares)
    : color_(color), shape_(shape), current_rotation_(0) {
    rotations_ = {squares};
    for (int i = 1; i < 4; ++i) {
        double rad = i * (-M_PI / 2);
        Squares rotated_squares = rotate(squares, pivot, rad);
        rotations_.push_back(rotated_squares);
    }
}

void Tetromino::rotateCW() {
    current_rotation_ = (current_rotation_ + 1) % 4;
}

void Tetromino::rotateCCW() {
    if (current_rotation_ == 0) {
        current_rotation_ = 3;
    } else {
        --current_rotation_;
    }
}

Tetromino::Color Tetromino::getColor() const { return color_; }

Tetromino::Shape Tetromino::getShape() const { return shape_; }

const Tetromino::Squares& Tetromino::getSquares() const {
    if (current_rotation_ % 4 < 0) {
        throw std::out_of_range("Negative rotation, cannot perform mod operation");
    }
    return rotations_[current_rotation_ % 4];
}

int Tetromino::getCurrentRotation() const { return current_rotation_; }

int Tetromino::setCurrentRotation(int rotation) {
    if (rotation < 0) {
        current_rotation_ = (rotation + abs(rotation * 4)) % 4;
    } else {
        current_rotation_ = rotation;
    }
    return current_rotation_;
}

std::string Tetromino::toString() const {
    std::string str = "";
    const int array_size = 4;
    int array_squares[array_size][array_size] = {{0}};
    for (const Square& square : getSquares()) {
        array_squares[square.first][square.second] = 1;
    }
    for (int i = array_size - 1; i >= 0; --i) {
        for (int j = 0; j < array_size; ++j) {
            if (array_squares[j][i] == 1) {
                str += "#";
            } else {
                str += "-";
            }
        }
        str += '\n';
    }
    return str;
}
