#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "tetris.hpp"

class TetrisBoard {
    using Board = std::vector<std::vector<sf::RectangleShape>>;

    //const sf::Color FINISHED_HUE_CHANGE = sf::Color(-20, -20, -20);
    const sf::Color FINISHED_HUE_CHANGE = sf::Color(-50, -50, -50);

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

    bool isStateFinished() const;

    void setStateFinished(bool finished);

private:
    bool state_finished_ = false;
    Board board_;
    sf::Vector2i board_tile_count_;
    TileProperties tile_prop_;
};

class GUI {
public:
    GUI(int width, int height);
    void update(const Tetris &tetris_human, const Tetris &tetris_ai);
    void draw();
    void close() { window_.close(); }
    bool pollEvent(sf::Event& event) { return window_.pollEvent(event); }

    static std::map<Tetromino::Color, sf::Color> color_map;

private:
    const sf::Color BG_COLOR = sf::Color(207, 185, 151);

    sf::RenderWindow window_;

    TetrisBoard board_human_;
    TetrisBoard board_ai_;
    TetrisBoard next_tetromino_panel_;

    sf::Font font_;
    sf::Text human_score_;
    sf::Text ai_score_;
};

#endif