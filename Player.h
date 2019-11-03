//
// Created by fourteener on 11/1/19.
//

#ifndef SFML_PRACTICE_PLAYER_H
#define SFML_PRACTICE_PLAYER_H

#include <SFML/System.hpp>

#include "Board.h"

// Player class to allow the player to move
class Player {
public:
    // Constructors
    Player();
    explicit Player(Board& board);

    // Movement and collision physics functions
    void move(bool left, bool right, bool up, bool down);
    void tickPhysics();

    // Get if the player object exists and where it exists
    bool isInitialized();
    sf::Vector2f getPos();

private:
    // Helper functions to make movement near walls feel better
    void wallSlide();
    void cornerSlip();

    // Movement variables
    double x, y, vx, vy, movementSpeed, deltaTime;

    // Does this player actually exist?
    bool initialized;

    // The game's gameboard
    Board gameboard;

    // Used to compute deltaTime
    sf::Clock movementClock;
};

#endif //SFML_PRACTICE_PLAYER_H
