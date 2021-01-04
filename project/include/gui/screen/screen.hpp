#ifndef GENETIC_TETRIS_SCREEN_HPP
#define GENETIC_TETRIS_SCREEN_HPP

#include <SFML/Graphics.hpp>

namespace gentetris {

class Screen {
public:
    Screen(sf::RenderWindow& window): window_(window) {
        if (!font_.loadFromFile(FONT_FILE)) {
            throw std::runtime_error("Error loading font!");
        }
    }
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void reset() = 0;
    virtual bool pollEvent(sf::Event& event) = 0;

protected:
    const sf::Color BG_COLOR = sf::Color(207, 185, 151);
    const std::string FONT_FILE = "res/HackBoldNerdFontCompleteMono.ttf";
    const int FONT_SIZE = 24;

    sf::RenderWindow& window_;
    sf::Font font_;
};

}

#endif  // GENETIC_TETRIS_SCREEN_HPP
