/*
 * Author: Rafal Kulus
 */

#include <iostream>

#include "tetris/tetris.hpp"

int main(){
    genetic_tetris::Tetris tetris;
    while(true){
        int o;
        std::cin>>o;
        switch(o){
            case 4:
                tetris.shiftLeft();
                break;
            case 6:
                tetris.shiftRight();
                break;
            case 2:
                tetris.hardDrop();
                break;
            case 5:
                tetris.rotateCW();
                break;
            default:
                tetris.tick();
        }
        std::cout<<tetris.toString();
    }
    return 0;
}