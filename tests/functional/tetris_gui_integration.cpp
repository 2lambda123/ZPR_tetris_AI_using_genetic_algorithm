/*
 * Author: Damian Kolaska
 */

#include "gui/gui.hpp"
#include "tetris/tetris.hpp"

int main() {
    using namespace genetic_tetris;
    bool closed = false;
    sf::Clock clock;
    Tetris tetris;
    EvolutionaryAlgo ai(tetris);
    GUI gui(800, 900, 60, tetris, tetris, ai);
    gui.setActiveScreen(GUI::ScreenType::GAME);

    clock.restart();
    while (!closed) {
        sf::Event event;
        while (gui.pollEvent(event)) {
            if (event.type == sf::Event::Closed) closed = true;
        }
        if (clock.getElapsedTime() > sf::seconds(1.0f)) {
            tetris.tick();
            clock.restart();
        }
        gui.update();
        gui.draw();
    }
    return 0;
}