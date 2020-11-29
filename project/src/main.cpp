#include "tetromino.hpp"
#include "tetromino_generator.hpp"

#include <iostream>

int main(){
    TetrominoGenerator gen;
    while(true){
        Tetromino t = gen.getTetromino();
        for(int i = 0; i < 4; ++i){
            std::string temp;
            std::cin>>temp;
            std::cout<<t.toString();
            t.rotateCW();
        }
    }
    return 0;
}