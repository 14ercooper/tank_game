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
    Player(Board& board);
    void jump();
    void move(double left, double right);
    void die();
    bool initialized;
    sf::Vector2f getPos();

private:
    double x, y;
    double vx, vy, gravity;
    Board gameboard;
    sf::Clock movementClock;
};

#endif //SFML_PRACTICE_PLAYER_H
