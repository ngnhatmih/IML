#include "game.h"
#include <iostream>

int main() {
    Game &game = Game::getInstance();

    if (!game.init()) return EXIT_GAME_FAILURE;
    game.processData();

    while(game.getRunningState()) {
        game.processEvent();
        game.render();
    }

    game.clean();

    return EXIT_GAME_SUCCESS;
}