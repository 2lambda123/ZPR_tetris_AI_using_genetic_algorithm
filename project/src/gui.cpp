#include "gui.hpp"

#include <cassert>

GUI::GUI(int width, int height)
    : window(sf::VideoMode(width, height), "Tetris AI"),
      shape(100.0f),
      tetris_board(sf::Vector2f(0, 0), sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_HEIGHT),
                   TetrisBoard::TileProperties(32, 1)) {
    shape.setFillColor(sf::Color::Green);
}

void GUI::draw() {
    window.clear();
    // window.draw(shape);
    tetris_board.draw(window);
    window.display();
}

TetrisBoard::TetrisBoard(const sf::Vector2f& position, const sf::Vector2i& board_tile_count,
                         const TileProperties& tile_prop)
    : board_tile_count(board_tile_count), board(board_tile_count.y), tile_prop(tile_prop) {
    for (int y = 0; y < board_tile_count.y; ++y) {
        for (int x = 0; x < board_tile_count.x; ++x) {
            board[y].push_back(sf::RectangleShape(sf::Vector2f(tile_prop.size, tile_prop.size)));
            board[y][x].setFillColor(sf::Color::Blue);
            sf::Vector2f tile_pos(tile_prop.padding + position.x + x * tile_prop.padded_size,
                                  tile_prop.padding + position.y + y * tile_prop.padded_size);
            board[y][x].setPosition(tile_pos);
        }
    }
}

void TetrisBoard::setState(const TetrisGrid& tetris_grid) {
    //assert(tetris_grid.size() == board_tile_count.y);
    //assert(tetris_grid[0].size() == board_tile_count.x);
    for (int y = 0; y < board_tile_count.y; ++y) {
        int x = 0;
        for (auto tile : tetris_grid[y]) {
            int mapped_y = board_tile_count.y - y - 1;
            if (tile != Tetromino::Color::EMPTY) {
                board[mapped_y][x].setFillColor(sf::Color::Red);
            }
            else {
                board[mapped_y][x].setFillColor(sf::Color::Blue);
            }
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