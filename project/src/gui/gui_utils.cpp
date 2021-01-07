#include "gui/gui_utils.hpp"

#include <iostream>

#include "sound_manager.hpp"

namespace genetic_tetris {

const std::map<Tetromino::Color, sf::Color> &getTetrominoColorMap() {
    static const std::map<Tetromino::Color, sf::Color> TETROMINO_COLOR_MAP = {
        {Tetromino::Color::EMPTY, sf::Color(255, 250, 250)},
        {Tetromino::Color::CYAN, sf::Color(0x00bcd4ff)},
        {Tetromino::Color::YELLOW, sf::Color(0xffeb3bff)},
        {Tetromino::Color::PURPLE, sf::Color(0x9c27b0ff)},
        {Tetromino::Color::GREEN, sf::Color(0x4caf50ff)},
        {Tetromino::Color::RED, sf::Color(0xf44336ff)},
        {Tetromino::Color::BLUE, sf::Color(0x2196f3ff)},
        {Tetromino::Color::ORANGE, sf::Color(0xff9800ff)},
        {Tetromino::Color::GHOST, sf::Color(0xcfd8dcff)}};
    return TETROMINO_COLOR_MAP;
}

TetrisBoard::TetrisBoard(const sf::Vector2f &position, const sf::Vector2i &board_tile_count,
                         const TileProperties &tile_prop)
    : state_finished_(false), board_(board_tile_count.y), board_tile_count_(board_tile_count) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        for (int x = 0; x < board_tile_count.x; ++x) {
            board_[y].push_back(sf::RectangleShape(sf::Vector2f(tile_prop.size, tile_prop.size)));
            board_[y][x].setFillColor(getTetrominoColorMap().at(Tetromino::Color::EMPTY));
            sf::Vector2f tile_pos(
                tile_prop.padding + position.x + (float)x * tile_prop.padded_size,
                tile_prop.padding + position.y + (float)y * tile_prop.padded_size);
            board_[y][x].setPosition(tile_pos);
        }
    }
}

void TetrisBoard::setState(const Tetris::Grid &tetris_grid) {
    for (int y = 0; y < board_tile_count_.y; ++y) {
        int x = 0;
        for (auto tile_color : tetris_grid[y]) {
            int mapped_y = board_tile_count_.y - y - 1;
            auto base_color = getTetrominoColorMap().at(tile_color);
            sf::Color result_color = base_color;
            if (state_finished_) {
                result_color = base_color - FINISHED_HUE_CHANGE_;
            }
            board_[mapped_y][x].setFillColor(result_color);
            ++x;
        }
    }
}

void TetrisBoard::setTetrominoQueue(const std::deque<Tetromino> &queue) {
    const int TETROMINO_GAP = 4;
    const int TETROMINO_SIZE = 4;
    int y = 0;
    int start_x = (board_tile_count_.x - TETROMINO_SIZE) / 2;
    int start_y = (TETROMINO_GAP - TETROMINO_SIZE) / 2;
    // clear board
    sf::Color empty = getTetrominoColorMap().at(Tetromino::Color::EMPTY);
    for (auto &row : board_) {
        for (auto &rect : row) {
            rect.setFillColor(empty);
        }
    }
    // draw tetrominoes
    for (const Tetromino &tetromino : queue) {
        if (y + TETROMINO_GAP >= board_tile_count_.y) {
            break;
        }
        Tetromino::Color color = tetromino.getColor();
        sf::Color mapped_color = getTetrominoColorMap().at(color);
        for (const Tetromino::Square &square : tetromino.getSquares()) {
            int t_x = start_x + square.first;
            int t_y = y + start_y + square.second;
            if (t_x >= 0 && t_y >= 0 && t_x < board_tile_count_.x && t_y < board_tile_count_.y) {
                board_[t_y][t_x].setFillColor(mapped_color);
            }
        }
        y += TETROMINO_GAP;
    }
}

void TetrisBoard::draw(sf::RenderWindow &window) {
    for (const auto &row : board_) {
        for (const auto &tile : row) {
            window.draw(tile);
        }
    }
}

void TetrisBoard::reset() { setStateFinished(false); }

bool TetrisBoard::isStateFinished() const { return state_finished_; }

void TetrisBoard::setStateFinished(bool finished) { state_finished_ = finished; }

Button::Button() : sound_manager_(SoundManager::getInstance()) {}

void Button::setPosition(const sf::Vector2f &pos) {
    text_.setPosition(pos.x, pos.y);
    rect_.setPosition(pos);
}

void Button::setSize(const sf::Vector2f &size) { rect_.setSize(size); }

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect_, states);
    target.draw(text_, states);
}

void Button::setText(const std::string &text, const sf::Font &font, int size) {
    font_ = font;
    text_.setFont(font_);
    text_.setCharacterSize(size);
    text_.setString(text);
    auto bounds = text_.getLocalBounds();
    auto rect_size = rect_.getSize();
    auto rect_pos = rect_.getPosition();
    float x = rect_pos.x + (rect_size.x - bounds.width) / 2.0f - bounds.left;
    float y = rect_pos.y + (rect_size.y - bounds.height) / 2.0f - bounds.top;
    text_.setPosition(x, y);
}

void Button::update() {
    if (state_ == State::CLICKED && clock_.getElapsedTime() < CLICK_ANIMATION_TIME_) {
        text_.setFillColor(text_color_ - CLICK_HUE_CHANGE_);
        rect_.setFillColor(bg_color_ - CLICK_HUE_CHANGE_);
    } else {
        state_ = State::NORMAL;
        text_.setFillColor(text_color_);
        rect_.setFillColor(bg_color_);
    }
}

void Button::handleEvent(const sf::Event &e, const sf::Window &window) {
    if (e.type == sf::Event::MouseButtonPressed) {
        auto mouse_pos = sf::Mouse::getPosition(window);
        auto button_bounds = rect_.getGlobalBounds();
        if (mouse_pos.x >= button_bounds.left &&
            mouse_pos.x <= button_bounds.left + button_bounds.width &&
            mouse_pos.y >= button_bounds.top &&
            mouse_pos.y <= button_bounds.top + button_bounds.height) {
            on_click_();
            state_ = State::CLICKED;
            clock_.restart();
            sound_manager_.play(SoundManager::Sound::CLICK);
        }
    }
}

void Button::setOnClick(std::function<void()> on_click) { on_click_ = std::move(on_click); }

void IncDecDialog::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(plus_button_, states);
    target.draw(minus_button_, states);
    target.draw(value_text_, states);
}

IncDecDialog::IncDecDialog()
    : button_size_(BUTTONS_DEFAULT_SIZE_), value_(0), font_size_(FONT_DEFAULT_SIZE_) {}

IncDecDialog &IncDecDialog::setPosition(const sf::Vector2f &pos) {
    dialog_pos_ = pos;
    return *this;
}

IncDecDialog &IncDecDialog::setFont(const sf::Font &font, int size) {
    font_ = font;
    font_size_ = size;
    return *this;
}

IncDecDialog &IncDecDialog::setValueBounds(const sf::Vector2i &bounds) {
    value_bounds_ = bounds;
    return *this;
}

void IncDecDialog::update() {
    plus_button_.update();
    minus_button_.update();
}

void IncDecDialog::handleEvent(const sf::Event &e, const sf::Window &window) {
    plus_button_.handleEvent(e, window);
    minus_button_.handleEvent(e, window);
    value_text_.setString(std::to_string(value_));
}

int IncDecDialog::getValue() const { return value_; }

void IncDecDialog::build() {
    plus_button_.setSize(button_size_);
    minus_button_.setSize(button_size_);
    value_text_.setFont(font_);
    value_text_.setCharacterSize(font_size_);
    value_text_.setPosition(dialog_pos_ + VALUE_TEXT_RELATIVE_POS_);
    plus_button_.setPosition(dialog_pos_ + PLUS_BUTTON_RELATIVE_POS_);
    minus_button_.setPosition(dialog_pos_ + MINUS_BUTTON_RELATIVE_POS_);
    plus_button_.setText("+", font_, font_size_);
    minus_button_.setText("-", font_, font_size_);
    value_ = std::clamp(value_, value_bounds_.x, value_bounds_.y);
    plus_button_.setOnClick([this]() {
        ++value_;
        value_ = std::clamp(value_, value_bounds_.x, value_bounds_.y);
    });
    minus_button_.setOnClick([this]() {
        --value_;
        value_ = std::clamp(value_, value_bounds_.x, value_bounds_.y);
    });
}

}  // namespace genetic_tetris
