#include "gui/screen/game_screen.hpp"

#include <iomanip>
#include <event_manager.hpp>

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
                            TetrisBoard::TileProperties(20.0f, 0.5f)) {
    createAIScore();
    createHumanScore();
    createHumanLevel();
    createHumanLevelSpeed();
    createHumanLevelProgress();
    createRestartButton();
    createBackButton();
}

void GameScreen::update() {
    if (tetris_human_.isFinished() && !board_human_.isStateFinished())
        board_human_.setStateFinished(true);
    if (tetris_ai_.isFinished() && !board_ai_.isStateFinished()) board_ai_.setStateFinished(true);
    // TODO: setting state happens in every frame even though it doesn't change that often.
    //  Possible optimization to consider if we have time.
    board_human_.setState(tetris_human_.getDisplayGrid());
    board_ai_.setState(tetris_ai_.getDisplayGrid());
    next_tetromino_panel_.setTetrominoQueue(tetris_human_.getTetrominoQueue());
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
    if (tetris_human_.isFinished())
        restart_button_.update();
    back_button_.update();
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
    if (tetris_human_.isFinished())
        window_.draw(restart_button_);
    window_.draw(back_button_);
    window_.display();
}

void GameScreen::reset() {
    board_human_.reset();
    board_ai_.reset();
}

bool GameScreen::pollEvent(sf::Event& event) {
    bool event_polled = window_.pollEvent(event);
    if (event_polled) {
        restart_button_.handleEvent(event, window_);
        back_button_.handleEvent(event, window_);
        return true;
    }
    return false;
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

void GameScreen::createRestartButton() {
    restart_button_.setPosition(sf::Vector2f(300, 400));
    restart_button_.setSize(sf::Vector2f(200, 50));
    restart_button_.setText("RESTART", font_);
    restart_button_.setOnClick([]() {
        EventManager::getInstance().addEvent(EventType::RESTART_BUTTON_CLICKED);
    });
}

void GameScreen::createBackButton() {
    back_button_.setPosition(sf::Vector2f(300, 800));
    back_button_.setSize(sf::Vector2f(200, 50));
    back_button_.setText("BACK", font_);
    back_button_.setOnClick([]() {
      EventManager::getInstance().addEvent(EventType::BACK_BUTTON_CLICKED);
    });
}

}  // namespace gentetris