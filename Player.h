//
// Created by fourteener on 11/1/19.
//

#ifndef SFML_PRACTICE_PLAYER_H
#define SFML_PRACTICE_PLAYER_H

#include <SFML/System.hpp>

#include "Board.h"

class Player {
public:
    Player();
    explicit Player(Board& board);
    void move(bool left, bool right, bool up, bool down);
    void die();
    void tickPhysics();
    bool isInitialized();
    sf::Vector2f getPos();

private:
    double x, y, vx, vy, movementSpeed, deltaTime;
    bool initialized;
    Board gameboard;
    sf::Clock movementClock;
};

#endif //SFML_PRACTICE_PLAYER_H
