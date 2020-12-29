#include "gui.hpp"

#include <cassert>

std::map<Tetromino::Color, sf::Color> GUI::color_map = {
        {Tetromino::EMPTY,  sf::Color(255, 250, 250)},
        {Tetromino::CYAN,   sf::Color(0x00bcd4ff)},
        {Tetromino::YELLOW, sf::Color(0xffeb3bff)},
        {Tetromino::PURPLE, sf::Color(0x9c27b0ff)},
        {Tetromino::GREEN,  sf::Color(0x4caf50ff)},
        {Tetromino::RED,    sf::Color(0xf44336ff)},
        {Tetromino::BLUE,   sf::Color(0x2196f3ff)},
        {Tetromino::ORANGE, sf::Color(0xff9800ff)}};

GUI::GUI(int width, int height)
        : window_(sf::VideoMode(width, height), "Tetris AI"),
          board_human_(sf::Vector2f(10, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                       TetrisBoard::TileProperties(30.0f, 0.5f)),
          board_ai_(sf::Vector2f(480, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                    TetrisBoard::TileProperties(30.0f, 0.5f)),
          next_tetromino_panel_(sf::Vector2f(337, 10), sf::Vector2i(6, 18),
                                TetrisBoard::TileProperties(20.0f, 0.5f)) {
    if (!font_.loadFromFile("res/HackBoldNerdFontCompleteMono.ttf")) {
        throw std::runtime_error("Error loading font!");
    }
    human_score_.setFont(font_);
    ai_score_.setFont(font_);
    human_score_.setString("Human: 0");
    ai_score_.setString("AI: 0");
    human_score_.setCharacterSize(24);
    ai_score_.setCharacterSize(24);
    human_score_.setPosition(sf::Vector2f(10, 640));
    ai_score_.setPosition(sf::Vector2f(480, 640));
}

void GUI::update(const Tetris &tetris_human, const Tetris &tetris_ai) {
    if (tetris_human.isFinished() && !board_human_.isStateFinished())
        board_human_.setStateFinished(true);
    if (tetris_ai.isFinished() && !board_ai_.isStateFinished())
        board_ai_.setStateFinished(true);
    board_human_.setState(tetris_human.getGrid());
    board_ai_.setState(tetris_ai.getGrid());
    human_score_.setString("Human: " + std::to_string(tetris_human.getScore()));
    ai_score_.setString("AI: " + std::to_string(tetris_ai.getScore()));
}

void GUI::draw() {
    window_.clear(BG_COLOR);
    board_human_.draw(window_);
    board_ai_.draw(window_);
    next_tetromino_panel_.draw(window_);
    window_.draw(human_score_);
    window_.draw(ai_score_);
    window_.display();
}

TetrisBoard::TetrisBoard(const sf::Vector2f &position, const sf::Vector2i &board_tile_count,
                         const TileProperties &tile_prop)
        : board_(board_tile_count.y), board_tile_count_(board_tile_count), tile_prop_(tile_prop) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        for (int x = 0; x < board_tile_count.x; ++x) {
            board_[y].push_back(sf::RectangleShape(sf::Vector2f(tile_prop.size, tile_prop.size)));
            board_[y][x].setFillColor(GUI::color_map.at(Tetromino::EMPTY));
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
            auto base_color = GUI::color_map.at(tile_color);
            sf::Color result_color = base_color;
            if (state_finished_) {
                result_color = sf::Color(
                        base_color.r + FINISHED_HUE_CHANGE.r,
                        base_color.g + FINISHED_HUE_CHANGE.g,
                        base_color.b + FINISHED_HUE_CHANGE.b
                );
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

bool TetrisBoard::isStateFinished() const {
    return state_finished_;
}

void TetrisBoard::setStateFinished(bool finished) {
    state_finished_ = finished;
}
