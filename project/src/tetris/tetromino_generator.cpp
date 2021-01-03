#include "tetris/tetromino_generator.hpp"

#include <cstdlib>
#include <utility>
#include <vector>
#include <deque>

#include "tetris/tetromino.hpp"

const std::vector<Tetromino> TetrominoGenerator::TETROMINOES = {
    Tetromino(Tetromino::Color::CYAN, Tetromino::Shape::I, std::pair(1.5, 1.5),
              {std::pair(0, 2), std::pair(1, 2), std::pair(2, 2), std::pair(3, 2)}),
    Tetromino(Tetromino::Color::BLUE, Tetromino::Shape::J, std::pair(1, 1),
              {std::pair(0, 2), std::pair(0, 1), std::pair(1, 1), std::pair(2, 1)}),
    Tetromino(Tetromino::Color::ORANGE, Tetromino::Shape::L, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(2, 1), std::pair(2, 2)}),
    Tetromino(Tetromino::Color::YELLOW, Tetromino::Shape::O, std::pair(1.5, 1.5),
              {std::pair(1, 2), std::pair(2, 2), std::pair(1, 1), std::pair(2, 1)}),
    Tetromino(Tetromino::Color::GREEN, Tetromino::Shape::S, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(1, 2), std::pair(2, 2)}),
    Tetromino(Tetromino::Color::PURPLE, Tetromino::Shape::T, std::pair(1, 1),
              {std::pair(0, 1), std::pair(1, 1), std::pair(2, 1), std::pair(1, 2)}),
    Tetromino(Tetromino::Color::RED, Tetromino::Shape::Z, std::pair(1, 1),
              {std::pair(0, 2), std::pair(1, 2), std::pair(1, 1), std::pair(2, 1)}),
};

TetrominoGenerator::TetrominoGenerator() : last_tetromino_(Tetromino()){
    generateTetrominoes();
}

Tetromino TetrominoGenerator::getNextTetromino() {
    last_tetromino_ = queue_.front();
    queue_.pop_front();
    if(queue_.size() < QUEUE_LENGTH){
        generateTetrominoes();
    }
    return last_tetromino_;
}

Tetromino TetrominoGenerator::getLastTetromino() const{
    return last_tetromino_;
}

std::deque<Tetromino> TetrominoGenerator::getQueue() const{
    return std::deque<Tetromino>(queue_.begin(), queue_.begin()+QUEUE_LENGTH);
}

void TetrominoGenerator::generateTetrominoes(){
    while(queue_.size() < QUEUE_LENGTH){
        std::vector<Tetromino> bag(TETROMINOES);
        while(!bag.empty()){
            unsigned int item_idx = rand() % bag.size();
            queue_.push_back(bag[item_idx]);
            bag.erase(bag.begin()+item_idx);
        }
    }
}