#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "tetris.hpp"

class TetrisBoard {
    using Board = std::vector<std::vector<sf::RectangleShape>>;

public:
    struct TileProperties {
        TileProperties(int size, int padding) : size(size), padding(padding) {
            padded_size = size + 2*padding;
        }
        int size;
        int padding;
        int padded_size;
    };

    TetrisBoard(const sf::Vector2f& position, const sf::Vector2i& board_tile_count,
                const TileProperties& tile_prop);
    void setState(const TetrisGrid& tetris_grid);
    void draw(sf::RenderWindow& window);

private:
    Board board;
    sf::Vector2i board_tile_count;
    TileProperties tile_prop;
};

class GUI {
public:
    GUI(int width, int height);
    void update(const TetrisGrid tetris_grid) { tetris_board.setState(tetris_grid); }
    void draw();
    void close() { window.close(); }
    bool pollEvent(sf::Event& event) { return window.pollEvent(event); }

private:
    sf::RenderWindow window;
    sf::CircleShape shape;

    const sf::Color BG_COLOR;
    TetrisBoard tetris_board;
};

#endif