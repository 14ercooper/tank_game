//
// Created by fourteener on 11/1/19.
//

#include <cmath>

#include "Player.h"

// Blank player constructor
Player::Player () {
    x = -1, y = -1, vx = -1, vy = -1, movementSpeed = -1;
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
    movementSpeed = 12;

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
void Player::move(bool left, bool right, bool up, bool down) {
    deltaTime = movementClock.getElapsedTime().asMilliseconds() / 1000.0;
    if (left && right)
        vx = 0;
    else if (left)
        vx = -left * movementSpeed;
    else if (right)
        vx = right * movementSpeed;

    if (up && down)
        vy = 0;
    else if (up)
        vy = -up * movementSpeed;
    else if (down)
        vy = down * movementSpeed;

    // Prevents sqrt(2) speed boost
    if ((left ||right) && (up || down)) {
        vx /= sqrt(2);
        vy /= sqrt(2);
    }

    // And tick player physics
    tickPhysics();
}

// Perform a physics tick
void Player::tickPhysics() {
    // Enforce a minimum speed
    if (vx * vx < 1)
        vx = 0;
    if (vy * vy < 1)
        vy = 0;

    // Translate the player
    x += vx * deltaTime;
    y += vy * deltaTime;

    vx -= vx * deltaTime * 25;
    vy -= vy * deltaTime * 25;

    // Stop winding up in walls
    // AKA Poor man's collision detection
    while (true) {
        if (!gameboard.isColliding(x, y) && gameboard.isColliding(x + 1, y)) {
            x -= 0.01;
        } else if (!gameboard.isColliding(x + 1, y) && gameboard.isColliding(x, y)) {
            x += 0.01;
        } else if (!gameboard.isColliding(x, y) && gameboard.isColliding(x, y + 1)) {
            y -= 0.01;
        } else if (!gameboard.isColliding(x, y + 1) && gameboard.isColliding(x, y)) {
            y += 0.01;
        }
        else {
            break;
        }
    }

    // Restart the movement clock for next delta time
    movementClock.restart();
}

// The player has died
void Player::die() {

}