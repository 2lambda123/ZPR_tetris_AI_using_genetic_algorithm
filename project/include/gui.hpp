#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "tetris.hpp"

class TetrisBoard {
    using Board = std::vector<std::vector<sf::RectangleShape>>;

public:
    struct TileProperties {
        TileProperties(float size, float padding) : size(size), padding(padding) {
            padded_size = size + 2 * padding;
        }
        float size;
        float padding;
        float padded_size;
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
    void update(const TetrisGrid tetris_grid);
    void draw();
    void close() { window.close(); }
    bool pollEvent(sf::Event& event) { return window.pollEvent(event); }

    static std::map<Tetromino::Color, sf::Color> color_map;

private:
    sf::RenderWindow window;
    sf::CircleShape shape;

    const sf::Color BG_COLOR;
    TetrisBoard human_board;
    TetrisBoard ai_board;
    TetrisBoard next_tetromino_panel;

    sf::Font font;
    sf::Text human_score;
    sf::Text ai_score;
};

#endif