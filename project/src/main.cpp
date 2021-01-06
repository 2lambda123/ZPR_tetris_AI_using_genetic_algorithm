#include <iostream>
#include <cstdlib>
#include <ctime>

#include "app.hpp"

int main() {
    // TODO: use c++11 random
    srand(time(nullptr));
    genetic_tetris::App app;
    app.run();
}