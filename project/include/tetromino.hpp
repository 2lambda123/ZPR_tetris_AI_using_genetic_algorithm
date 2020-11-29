#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include <string>
#include <utility>
#include <vector>

class Tetromino{
public:
    enum Color { EMPTY, CYAN, YELLOW, PURPLE, GREEN, RED, BLUE, ORANGE };
    enum Shape { NO_SHAPE, I, O, T, S, Z, J, L };

    Tetromino();
    Tetromino(Color color, Shape shape, std::pair<double, double> pivot, std::vector<std::pair<int, int> > squares);
    Tetromino(const Tetromino &tetromino) = default;
    Tetromino& operator=(const Tetromino &tetromino);
    void rotateCW();
    void rotateCCW();
    Color getColor() const;
    Shape getShape() const;
    const std::pair<double, double>& getPivot() const;
    const std::vector<std::pair<int, int> >& getSquares() const;
    std::string toString() const;

private:
    Color color_;
    Shape shape_;
    std::pair<double, double> pivot_;
    std::vector<std::pair<int, int> > squares_;

    void rotate(double rad);
};

#endif