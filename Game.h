//
// Created by fourteener on 10/31/19.
//

#ifndef SFML_PRACTICE_GAME_H
#define SFML_PRACTICE_GAME_H

#include <SFML/Graphics.hpp>
#include "Board.h"

// The core class for the game
class Game {
public:
    // Basically pull the game loop out of main
    // This makes main really easy to read, and is more OOP-based
    // Also means multiple games could be launched from a single main
    void run(int windowSize, int tileSize);
};

#endif //SFML_PRACTICE_GAME_H
