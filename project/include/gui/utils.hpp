#ifndef GENETIC_TETRIS_UTILS_HPP
#define GENETIC_TETRIS_UTILS_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>

#include "tetris/tetris.hpp"

namespace gentetris {

const std::map<Tetromino::Color, sf::Color> TETROMINO_COLOR_MAP = {
    {Tetromino::EMPTY, sf::Color(255, 250, 250)}, {Tetromino::CYAN, sf::Color(0x00bcd4ff)},
    {Tetromino::YELLOW, sf::Color(0xffeb3bff)},   {Tetromino::PURPLE, sf::Color(0x9c27b0ff)},
    {Tetromino::GREEN, sf::Color(0x4caf50ff)},    {Tetromino::RED, sf::Color(0xf44336ff)},
    {Tetromino::BLUE, sf::Color(0x2196f3ff)},     {Tetromino::ORANGE, sf::Color(0xff9800ff)}};

class TetrisBoard {
    using Board = std::vector<std::vector<sf::RectangleShape>>;

    // const sf::Color FINISHED_HUE_CHANGE = sf::Color(-20, -20, -20);
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

class Button : public sf::Drawable, public Subject {
public:
    Button(const sf::Vector2f& pos, const sf::Vector2f& size);
    void setText(const std::string& text, const sf::Font& font, int size = 24);
    void update(const sf::Event& e);
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Color text_color_ = sf::Color::White;
    sf::Color bg_color_ = sf::Color(0x708090ff);
    sf::Text text_;
    sf::RectangleShape rect_;
};

}  // namespace gentetris

#endif  // GENETIC_TETRIS_UTILS_HPP
