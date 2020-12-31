#include "gui/utils.hpp"

#include <iostream>

namespace gentetris {

TetrisBoard::TetrisBoard(const sf::Vector2f &position, const sf::Vector2i &board_tile_count,
                         const TileProperties &tile_prop)
    : board_(board_tile_count.y), board_tile_count_(board_tile_count), tile_prop_(tile_prop) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        for (int x = 0; x < board_tile_count.x; ++x) {
            board_[y].push_back(sf::RectangleShape(sf::Vector2f(tile_prop.size, tile_prop.size)));
            board_[y][x].setFillColor(TETROMINO_COLOR_MAP.at(Tetromino::EMPTY));
            sf::Vector2f tile_pos(tile_prop.padding + position.x + x * tile_prop.padded_size,
                                  tile_prop.padding + position.y + y * tile_prop.padded_size);
            board_[y][x].setPosition(tile_pos);
        }
    }
}

void TetrisBoard::setState(const Tetris::Grid &tetris_grid) {
    for (int y = 0; y < board_tile_count_.y; ++y) {
        int x = 0;
        for (auto tile_color : tetris_grid[y]) {
            int mapped_y = board_tile_count_.y - y - 1;
            auto base_color = TETROMINO_COLOR_MAP.at(tile_color);
            sf::Color result_color = base_color;
            if (state_finished_) {
                result_color = base_color - FINISHED_HUE_CHANGE;
            }
            board_[mapped_y][x].setFillColor(result_color);
            ++x;
        }
    }
}

void TetrisBoard::draw(sf::RenderWindow &window) {
    for (auto row : board_) {
        for (auto tile : row) {
            window.draw(tile);
        }
    }
}

bool TetrisBoard::isStateFinished() const { return state_finished_; }

void TetrisBoard::setStateFinished(bool finished) { state_finished_ = finished; }

Button::Button(const sf::Vector2f &pos, const sf::Vector2f &size) {
    text_.setPosition(pos.x, pos.y);
    rect_.setPosition(pos);
    rect_.setSize(size);
    if (!buffer_.loadFromFile(BUTTON_CLICK_SOUND))
        throw std::runtime_error("Cannot open " + BUTTON_CLICK_SOUND);
    sound_.setBuffer(buffer_);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect_, states);
    target.draw(text_, states);
}

void Button::setText(const std::string &text, const sf::Font &font, int size) {
    text_.setFont(font);
    text_.setCharacterSize(size);
    text_.setString(text);
    auto bounds = text_.getLocalBounds();
    auto rect_size = rect_.getSize();
    auto rect_pos = rect_.getPosition();
    int x = rect_pos.x + (rect_size.x -bounds.width) / 2.0f - bounds.left;
    int y = rect_pos.y + (rect_size.y - bounds.height) / 2.0f - bounds.top;
    text_.setPosition(x, y);
}

void Button::update() {
    if (state_ == State::CLICKED && clock_.getElapsedTime() < CLICK_ANIMATION_TIME) {
        text_.setFillColor(text_color_ - CLICK_HUE_CHANGE);
        rect_.setFillColor(bg_color_ - CLICK_HUE_CHANGE);
    }
    else {
        state_ = State::NORMAL;
        text_.setFillColor(text_color_);
        rect_.setFillColor(bg_color_);
    }
}

void Button::handleEvent(const sf::Event &e, const sf::Window &window) {
    if (e.type == sf::Event::MouseButtonPressed) {
        auto mouse_pos = sf::Mouse::getPosition(window);
        auto button_bounds = rect_.getGlobalBounds();
        if (mouse_pos.x >= button_bounds.left && mouse_pos.x <= button_bounds.left + button_bounds.width &&
            mouse_pos.y >= button_bounds.top && button_bounds.top + button_bounds.height) {
            notifyObservers(GenTetrisEvent::PLAY_BUTTON_CLICKED);
            state_ = State::CLICKED;
            clock_.restart();
            sound_.play();
        }
    }
}


}

