#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <string>
#include <utility>
#include <vector>

class Tetromino{
public:
    enum Color { CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE };
    enum Shape { I, O, T, S, Z, J, L };

    Tetromino(Color color, Shape shape, std::pair<double, double> pivot, std::vector<std::pair<int, int> > squares);
    Tetromino(const Tetromino &tetromino) = default;
    void rotateCW();
    void rotateCCW();
    const std::pair<double, double>& getPivot() const;
    const std::vector<std::pair<int, int> >& getSquares() const;
    std::string toString() const;

private:
    const Color color_;
    const Shape shape_;
    const std::pair<double, double> pivot_;
    std::vector<std::pair<int, int> > squares_;

    void rotate(double rad);
};

#endif