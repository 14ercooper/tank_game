/* Wii Tanks Ghost Bullet Hell
 *
 * (c) 2019 Logan Cooper
 * All Rights Reserved
 *
 * A video game made using C++ and SFML. It's hard. Really hard. I made it to level 9 and I made the thing. Good luck.
 */

#include <cstdlib>
#include <ctime>

#include "Game.h"

int main() {
    // Seed the random number generator
    std::srand(std::time(0));
    std::rand();

    // Window and tile sizes are defined here
    // More tiles in the window means levels generate slower but the game is somewhat easier.
    // I find these values to be pretty good.
    const int windowSize = 1024;
    int tileSize = 16;

    // Create a game instance
    Game game;

    // Run the game; this calls the main game loop
    game.run(windowSize, tileSize);

    // Return a success
    return 0;
}