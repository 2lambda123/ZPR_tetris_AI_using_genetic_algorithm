#include "tetris.hpp"

#include <iostream>

int main(){
    Tetris tetris;
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