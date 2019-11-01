//
// Created by fourteener on 11/1/19.
//

#include <iostream>

#include "Player.h"

Player::Player () {
    x = -1, y = -1, vx = -1, vy = -1, gravity = -1;
    initialized = false;
};

Player::Player(Board &board) {
    gameboard = board;
    const int mapSize = board.getBoard().size();
    double initX, initY;
    // Get an air tile
    while (true) {
        initX = (rand() % mapSize);
        initY = (rand() % mapSize);
        if (!board.isColliding(initX, initY)) {
            break;
        }
    }

    // Move the player down to the floor
    while (true) {
        if (board.isColliding(initX, initY + 1))
            break;
        initY += 1;
    }

    // Define position and velocity
    x = initX;
    y = initY;
    vx = 0;
    vy = 0;

    std::cout << x << "," << y << "\n";

    // Define gravity
    gravity = 2;

    // Mark as initialized
    initialized = true;
}

sf::Vector2f Player::getPos() {
    return sf::Vector2f(x * gameboard.getTileSize(),y * gameboard.getTileSize());
}

void Player::jump() {

}

void Player::move(double left, double right) {

}

void Player::die() {

}