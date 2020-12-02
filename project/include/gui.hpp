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
    void setState(const Tetris::Grid& tetris_grid);
    void draw(sf::RenderWindow& window);

private:
    Board board_;
    sf::Vector2i board_tile_count_;
    TileProperties tile_prop_;
};

class GUI {
public:
    GUI(int width, int height);
    void update(const Tetris::Grid& human, const Tetris::Grid& ai);
    void draw();
    void close() { window_.close(); }
    bool pollEvent(sf::Event& event) { return window_.pollEvent(event); }

    static std::map<Tetromino::Color, sf::Color> color_map;

private:
    sf::RenderWindow window_;
    sf::CircleShape shape_;

    const sf::Color BG_COLOR_;
    TetrisBoard human_board_;
    TetrisBoard ai_board_;
    TetrisBoard next_tetromino_panel_;

    sf::Font font_;
    sf::Text human_score_;
    sf::Text ai_score_;
};

#endif