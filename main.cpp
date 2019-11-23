/* Basic graphical caves generator
 *
 * (c) 2019 Logan Cooper
 * All Rights Reserved
 */

#include <cstdlib>
#include <ctime>

#include "Game.h"

int main() {
    // Seed the random number generator
    std::srand(std::time(0));
    std::rand();

    // Window and tile sizes are defined here
    const int windowSize = 1024;
    int tileSize = 16;

    // Start the game
    Game game;
    game.run(windowSize, tileSize);

    // Return a success
    return 0;
}

/* TODO
 * Title screen
 * Save best level to file
 */