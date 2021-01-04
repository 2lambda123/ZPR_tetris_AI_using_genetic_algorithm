#include "gui/gui.hpp"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace gentetris {

GUI::GUI(int width, int height, int fps, Tetris& human_tetris, Tetris& ai_tetris)
    : window_(sf::VideoMode(width, height), "Tetris AI"),
      game_screen_(window_, human_tetris, ai_tetris){
    window_.setFramerateLimit(fps);
}

void GUI::update(const Tetris &tetris_human, const Tetris &tetris_ai) {
    game_screen_.update();
}

void GUI::draw() {
    game_screen_.draw();
}

bool GUI::pollEvent(sf::Event &event) {
    return game_screen_.pollEvent(event);
}

}  // namespace gentetris
