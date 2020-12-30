#include "gui.hpp"
#include "tetris/tetris.hpp"

int main() {
    bool closed = false;
    sf::Clock clock;
    GUI gui(800, 900);
    Tetris tetris;

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
        gui.update(tetris.getGrid(), tetris.getGrid());
        gui.draw();
    }
    return 0;
}