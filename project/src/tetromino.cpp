#include "tetromino.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <utility>
#include <vector>

using Pivot = std::pair<double, double>;
using Square = std::pair<int, int>;
using Squares = std::vector<Square>;

Tetromino::Tetromino()
    : color_(Color::EMPTY), shape_(Shape::NO_SHAPE), pivot_(std::pair(0, 0)), squares_({})
{}

Tetromino::Tetromino(Color color, Shape shape, Pivot pivot, Squares squares)
    : color_(color), shape_(shape), pivot_(pivot), squares_(squares)
{}

Tetromino& Tetromino::operator=(const Tetromino &tetromino){
    this->color_ = tetromino.color_;
    this->shape_ = tetromino.shape_;
    this->pivot_ = tetromino.pivot_;
    this->squares_ = tetromino.squares_;
    return *this;
}

void Tetromino::rotateCW(){
    rotate(-M_PI/2);
}

void Tetromino::rotateCCW(){
    rotate(M_PI/2);
}

Tetromino::Color Tetromino::getColor() const{
    return color_;
}

Tetromino::Shape Tetromino::getShape() const{
    return shape_;
}

const Pivot& Tetromino::getPivot() const{
    return pivot_;
}

const Squares& Tetromino::getSquares() const{
    return squares_;
}

std::string Tetromino::toString() const{
    std::string str = "";
    const int arraySize = 4;
    int arraySquares[arraySize][arraySize] = { {0} };
    for(const Square& square : squares_){
        arraySquares[square.first][square.second] = 1;
    }
    for(int i = arraySize-1; i >= 0; --i){
        for(int j = 0; j < arraySize; ++j){
            if(arraySquares[j][i] == 1){
                str += "#";
            } else{
                str += "-";
            }
        }
        str += '\n';
    }
    return str;
}

void Tetromino::rotate(double rad){
    Squares rotatedSquares;
    for(const Square& square : squares_){
        double x = square.first - pivot_.first;
        double y = square.second - pivot_.second;
        double newX = x*cos(rad) - y*sin(rad);
        double newY = y*cos(rad) + x*sin(rad);
        Square newSquare = std::make_pair(round(newX + pivot_.first), round(newY + pivot_.second));
        rotatedSquares.push_back(newSquare);
    }
    squares_ = rotatedSquares;
}