#include "gui/screen/game_screen.hpp"

#include <iomanip>

namespace gentetris {

GameScreen::GameScreen(sf::RenderWindow& window, const Tetris& tetris_human, const Tetris& tetris_ai)
    : Screen(window),
      tetris_human_(tetris_human),
      tetris_ai_(tetris_ai),
      board_human_(sf::Vector2f(10, 10),
                   sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_VISIBLE_HEIGHT),
                   TetrisBoard::TileProperties(30.0f, 0.5f)),
      board_ai_(sf::Vector2f(480, 10),
                sf::Vector2i(Tetris::GRID_WIDTH, Tetris::GRID_VISIBLE_HEIGHT),
                TetrisBoard::TileProperties(30.0f, 0.5f)),
      next_tetromino_panel_(sf::Vector2f(337, 10), sf::Vector2i(6, 18),
                            TetrisBoard::TileProperties(20.0f, 0.5f)),
      play_button_(sf::Vector2f(300, 800), sf::Vector2f(200, 50)) {
    createAIScore();
    createHumanScore();
    createHumanLevel();
    createHumanLevelSpeed();
    createHumanLevelProgress();
    createPlayButton();
}

void GameScreen::update() {
    if (tetris_human_.isFinished() && !board_human_.isStateFinished())
        board_human_.setStateFinished(true);
    if (tetris_ai_.isFinished() && !board_ai_.isStateFinished()) board_ai_.setStateFinished(true);
    board_human_.setState(tetris_human_.getDisplayGrid());
    board_ai_.setState(tetris_ai_.getDisplayGrid());
    human_score_.setString("Human: " + std::to_string(tetris_human_.getScore()));
    human_level_.setString("Level: " + std::to_string(tetris_human_.getLevel()) + "/" +
                           std::to_string(Tetris::MAX_LEVEL));
    human_level_progress_.setString(
        "Lv progress: " + std::to_string(tetris_human_.getLevelProgress()) + "/" +
        std::to_string(Tetris::LINES_PER_LEVEL));
    std::stringstream speed;
    speed << std::fixed << std::setprecision(5) << tetris_human_.getLevelSpeed();
    human_level_speed_.setString("Lv speed: " + speed.str() + " sec/line");
    ai_score_.setString("AI: " + std::to_string(tetris_ai_.getScore()));
    play_button_.update();
}

void GameScreen::draw() {
    window_.clear(BG_COLOR);
    board_human_.draw(window_);
    board_ai_.draw(window_);
    next_tetromino_panel_.draw(window_);
    window_.draw(human_score_);
    window_.draw(human_level_);
    window_.draw(human_level_progress_);
    window_.draw(human_level_speed_);
    window_.draw(ai_score_);
    window_.draw(play_button_);
    window_.display();
}

void GameScreen::reset() {
    board_human_.reset();
    board_ai_.reset();
}
bool GameScreen::pollEvent(sf::Event& event) {
    bool event_polled = window_.pollEvent(event);
    if (event_polled) {
        play_button_.handleEvent(event, window_);
        return true;
    }
    return false;
}

sf::Text GameScreen::createText(const sf::Vector2f& position, int font_size) {
    sf::Text text;
    text.setFont(font_);
    text.setCharacterSize(font_size);
    text.setPosition(position);
    return text;
}

void GameScreen::createHumanScore() {
    human_score_ = createText(sf::Vector2f(10, 640), FONT_SIZE);
}

void GameScreen::createAIScore() {
    ai_score_ = createText(sf::Vector2f(480, 640), FONT_SIZE);
}

void GameScreen::createHumanLevelProgress() {
    human_level_progress_ = createText(sf::Vector2f(10, 703), (int)(FONT_SIZE * 0.75));
}

void GameScreen::createHumanLevelSpeed() {
    human_level_speed_ = createText(sf::Vector2f(10, 730), (int)(FONT_SIZE * 0.75));
}

void GameScreen::createHumanLevel() {
    human_level_ = createText(sf::Vector2f(10, 670), FONT_SIZE);
}

void GameScreen::createPlayButton() {
    play_button_.setText("PLAY", font_);
}


}  // namespace gentetris