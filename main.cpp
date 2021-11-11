#include "Conway.h"
#include <iostream>

int main() {
    std::cout.sync_with_stdio(false);
    ConwayLifeGame game(30, 130);
    game.start();

    return 0;
}
