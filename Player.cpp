//
// Created by fourteener on 11/1/19.
//

#include <iostream>

#include "Player.h"

// Blank player constructor
Player::Player () {
    x = -1, y = -1, vx = -1, vy = -1, gravity = -1, movementSpeed = -1, drag = -1;
    initialized = false;
};

// Regular player constructor
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

    // Define gravity, movement speed, and drag
    gravity = 6;
    movementSpeed = 2;
    drag = 0.5;

    // Mark as initialized
    initialized = true;
}

// Is the player initialized?
bool Player::isInitialized() {
    return initialized;
}

// Get the player's position
sf::Vector2f Player::getPos() {
    return sf::Vector2f(x * gameboard.getTileSize(),y * gameboard.getTileSize());
}

// Move the player if the key is held by adding velocity
void Player::move(bool left, bool right) {
    double deltaTime = movementClock.getElapsedTime().asMilliseconds() / 1000.0;
    vx -= left * movementSpeed * deltaTime;
    vx += right * movementSpeed * deltaTime;

    // Makes it not feel like ice physics
    if (!left && !right) {
        vx *= 0.85;
    }

    // And tick player physics
    tickPhysics(deltaTime);
}

// Perform a physics tick
void Player::tickPhysics(double deltaTime) {
    // Add gravity, the maximum fall speed is about 0.28 based on framerate
    vy -= gravity * deltaTime;

    // Handle drag forces
    int xSign = (vx > 0) ? 1 : -1;
    int ySign = (vy > 0) ? 1 : -1;
    double xMag = vx * xSign;
    double yMag = vy * ySign;
    vx -= xSign * (xMag * xMag) * drag;
    vy -= ySign * (yMag * yMag) * drag;
    xSign = (vx > 0) ? 1 : -1;
    ySign = (vy > 0) ? 1 : -1;

    // Enforce a minimum speed
    if (vx * xSign < 0.02)
        vx = 0;
    if (vy * ySign < 0.02)
        vy = 0;

    // Handle collisions, including accounting for player size
    int tileSize = gameboard.getTileSize();
    bool yColl = false;
    if (ySign > 0) {
        while (gameboard.isColliding(x,y+vy*tileSize+1)) {
            yColl = true;
            vy -= 0.01;
        }
    }
    else if (ySign < 0) {
        while (gameboard.isColliding(x,y+vy*tileSize)) {
            yColl = true;
            vy += 0.01;
        }
    }

    // Translate the player
    x += vx;
    y += vy;

    // Restart the movement clock for next delta time
    movementClock.restart();
}

// Jump!
void Player::jump() {

}

// The player has died
void Player::die() {

}