#include <iostream>
#include <cstdlib>
#include <ctime>

#include "app.hpp"

int main() {
    srand(time(nullptr));
    gentetris::App app;
    app.run();
}