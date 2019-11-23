//
// Created by fourteener on 10/31/19.
//

#ifndef SFML_PRACTICE_GAME_H
#define SFML_PRACTICE_GAME_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include <vector>
#include "Weapon.h"
#include "Enemy.h"

// The core class for the game
class Game {
public:
    // Basically pull the game loop out of main
    // This makes main really easy to read, and is more OOP-based
    // Also means multiple games could be launched from a single main
    void run(int windowSize, int tileSize);

    // Returns the current game object
    static Game* getGame();

    // Add a weapon or enemy to the game
    void addWeapon(Weapon& weapon);
    void addEnemy(Enemy& enemy);
    void resetAddedObjects();

    // Gets the level
    int getLevel();

    // Checks for a collision with the board
    bool boardCollision(sf::Vector2f pos);

    // Gets the size of a time
    int getTileSize();
private:
    // Stores the current game object
    static Game* _theGame;

    // Stores a list of weapons and enemies currently in the game
    std::vector<Weapon> _weapons;
    std::vector<Enemy> _enemies;

    // What level are we on?
    int _level = 1;

    // The gameboard
    Board _board;
};

#endif //SFML_PRACTICE_GAME_H
