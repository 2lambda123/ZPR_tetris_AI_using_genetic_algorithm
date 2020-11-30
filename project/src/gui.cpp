#include "gui.hpp"

#include <cassert>

std::map<Tetromino::Color, sf::Color> GUI::color_map = {
    {Tetromino::EMPTY, sf::Color(255, 250, 250)},  {Tetromino::CYAN, sf::Color(0, 255, 255)},
    {Tetromino::YELLOW, sf::Color(255, 255, 102)}, {Tetromino::PURPLE, sf::Color(221, 160, 221)},
    {Tetromino::GREEN, sf::Color(107, 142, 35)},   {Tetromino::RED, sf::Color(220, 20, 60)},
    {Tetromino::BLUE, sf::Color(240, 248, 255)},   {Tetromino::ORANGE, sf::Color(255, 165, 0)}};

GUI::GUI(int width, int height)
    : window(sf::VideoMode(width, height), "Tetris AI"),
      shape(100.0f),
      human_board(sf::Vector2f(10, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                  TetrisBoard::TileProperties(30.0f, 0.5f)),
      ai_board(sf::Vector2f(480, 10), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
               TetrisBoard::TileProperties(30.0f, 0.5f)),
      next_tetromino_panel(sf::Vector2f(337, 10), sf::Vector2i(6, 18),
               TetrisBoard::TileProperties(20.0f, 0.5f)) {
    shape.setFillColor(sf::Color::Green);
    if (!font.loadFromFile("res/HackBoldNerdFontCompleteMono.ttf")) {
        throw std::runtime_error("Error loading font!");
    }
    human_score.setFont(font);
    ai_score.setFont(font);
    human_score.setString("Human: 0");
    ai_score.setString("AI: 0");
    human_score.setCharacterSize(24);
    ai_score.setCharacterSize(24);
    human_score.setPosition(sf::Vector2f(10, 640));
    ai_score.setPosition(sf::Vector2f(480, 640));
}

void GUI::update(const TetrisGrid tetris_grid) {
    human_board.setState(tetris_grid);
    ai_board.setState(tetris_grid);
}

void GUI::draw() {
    // window.clear(sf::Color(245, 245, 220));
    window.clear(sf::Color(207, 185, 151));
    // window.draw(shape);
    human_board.draw(window);
    ai_board.draw(window);
    next_tetromino_panel.draw(window);
    window.draw(human_score);
    window.draw(ai_score);
    window.display();
}

TetrisBoard::TetrisBoard(const sf::Vector2f& position, const sf::Vector2i& board_tile_count,
                         const TileProperties& tile_prop)
    : board_tile_count(board_tile_count), board(board_tile_count.y), tile_prop(tile_prop) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        for (int x = 0; x < board_tile_count.x; ++x) {
            board[y].push_back(sf::RectangleShape(sf::Vector2f(tile_prop.size, tile_prop.size)));
            board[y][x].setFillColor(GUI::color_map.at(Tetromino::EMPTY));
            sf::Vector2f tile_pos(tile_prop.padding + position.x + x * tile_prop.padded_size,
                                  tile_prop.padding + position.y + y * tile_prop.padded_size);
            board[y][x].setPosition(tile_pos);
        }
    }
}

void TetrisBoard::setState(const TetrisGrid& tetris_grid) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        int x = 0;
        for (auto tile_color : tetris_grid[y]) {
            int mapped_y = board_tile_count.y - y - 1;
            board[mapped_y][x].setFillColor(GUI::color_map.at(tile_color));
            ++x;
        }
    }
}

void TetrisBoard::draw(sf::RenderWindow& window) {
    for (auto row : board) {
        for (auto tile : row) {
            window.draw(tile);
        }
    }
}