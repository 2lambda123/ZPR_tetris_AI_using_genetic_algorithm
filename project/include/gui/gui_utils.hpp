/*
 * Author: Damian Kolaska
 */

#ifndef GENETIC_TETRIS_GUI_UTILS_HPP
#define GENETIC_TETRIS_GUI_UTILS_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <map>
#include <vector>

#include "sound_manager.hpp"
#include "tetris/tetris.hpp"
/**
 * This file contains helper classes used in GUI.
 */
namespace genetic_tetris {

const std::map<Tetromino::Color, sf::Color>& getTetrominoColorMap();

/**
 * Class used to display tetris grid and next tetromino panel.
 */
class TetrisBoard {
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
    void setTetrominoQueue(const std::deque<Tetromino>& queue);
    void draw(sf::RenderWindow& window);
    void reset();

    bool isStateFinished() const;
    void setStateFinished(bool finished);

private:
    using Board = std::vector<std::vector<sf::RectangleShape>>;

    const sf::Color FINISHED_HUE_CHANGE_ = sf::Color(50, 50, 50, 0);

    bool state_finished_;
    Board board_;
    sf::Vector2i board_tile_count_;
};

/**
 * Custom button class.
 * Provides with only most basic functionalities like custom click handlers, hue change on clicked.
 */
class Button : public sf::Drawable {
public:
    Button();
    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
    void setText(const std::string& text, const sf::Font& font, int size = 24);
    void update();
    void handleEvent(const sf::Event& e, const sf::Window& window);
    void setOnClick(std::function<void()> on_click);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Time CLICK_ANIMATION_TIME_ = sf::seconds(0.1f);
    const sf::Color CLICK_HUE_CHANGE_ = sf::Color(20, 20, 20, 0);

    enum class State {
        NORMAL,
        CLICKED,
    } state_ = State::NORMAL;

    sf::Color text_color_ = sf::Color::White;
    sf::Color bg_color_ = sf::Color(0x708090ff);
    sf::Text text_;
    sf::Font font_;
    sf::RectangleShape rect_;

    SoundManager& sound_manager_;

    sf::Clock clock_;

    std::function<void()> on_click_;
};

/**
 * Counter dialog consisting of plus and minus buttons and counter value.
 */
class IncDecDialog : public sf::Drawable {
public:
    IncDecDialog();
    IncDecDialog& setPosition(const sf::Vector2f& pos);
    IncDecDialog& setFont(const sf::Font& font, int size = 24);
    IncDecDialog& setValueBounds(const sf::Vector2i& bounds);
    void build();
    void update();
    void handleEvent(const sf::Event& e, const sf::Window& window);
    int getValue() const;
    void setValue(int value);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    const sf::Vector2f PLUS_BUTTON_RELATIVE_POS_ = sf::Vector2f(0, -20);
    const sf::Vector2f MINUS_BUTTON_RELATIVE_POS_ = sf::Vector2f(0, 24);
    const sf::Vector2f VALUE_TEXT_RELATIVE_POS_ = sf::Vector2f(0, 0);

    const sf::Vector2f BUTTONS_DEFAULT_SIZE_ = sf::Vector2f(20, 20);
    const int FONT_DEFAULT_SIZE_ = 24;

    sf::Vector2f dialog_pos_;
    sf::Vector2i value_bounds_;
    Button plus_button_;
    Button minus_button_;
    sf::Vector2f button_size_;
    sf::Text value_text_;
    int value_;

    sf::Font font_;
    int font_size_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_GUI_UTILS_HPP
