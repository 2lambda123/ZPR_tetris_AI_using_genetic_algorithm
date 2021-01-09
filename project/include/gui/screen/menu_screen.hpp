/*
 * Author: Damian Kolaska
 */

#ifndef GENETIC_TETRIS_MENU_SCREEN_HPP
#define GENETIC_TETRIS_MENU_SCREEN_HPP

#include "gui/gui_utils.hpp"
#include "screen.hpp"

namespace genetic_tetris {

class MenuScreen : public Screen {
public:
    explicit MenuScreen(sf::RenderWindow& window);
    void update() override;
    void draw() override;
    void reset() override {}
    void handleSfmlEvent(const sf::Event& event) override;
    void handleCustomEvent(EventType) override {}

private:
    void createPlayButton();
    void createEvolveButton();
    void createExitButton();

    Button play_button_;
    Button evolve_button_;
    Button exit_button_;
};

}  // namespace genetic_tetris

#endif  // GENETIC_TETRIS_MENU_SCREEN_HPP
