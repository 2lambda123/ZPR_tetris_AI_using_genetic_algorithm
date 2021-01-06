#include <iostream>
#include <cstdlib>
#include <ctime>

#include "app.hpp"

int main() {
    srand(time(nullptr));
    genetic_tetris::App app;
    app.run();
}