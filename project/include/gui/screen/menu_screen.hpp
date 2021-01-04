#ifndef GENETIC_TETRIS_MENU_SCREEN_HPP
#define GENETIC_TETRIS_MENU_SCREEN_HPP

#include "gui/gui_utils.hpp"
#include "screen.hpp"
namespace gentetris {

class MenuScreen : public Screen {
public:
    MenuScreen(sf::RenderWindow& window);
    void update() override;
    void draw() override;
    void reset() override;
    bool pollEvent(sf::Event& event) override;
private:
    void createPlayButton();
    void createEvolveButton();
    void createExitButton();

    Button play_button_;
    Button evolve_button_;
    Button exit_button_;
};

}
#endif  // GENETIC_TETRIS_MENU_SCREEN_HPP
