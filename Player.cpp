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

    // Define movement speed and collision variables
    movementSpeed = 12;
    didMovementCorrect = false;

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
    double oldX = x;
    double oldY = y;
    x += vx * deltaTime;
    y += vy * deltaTime;

    // Stop winding up in walls
    // AKA Poor man's collision detection
    if (gameboard.isColliding(x + 0.02,y + 0.02)
        || gameboard.isColliding(x + 0.02,y + 0.98)
        || gameboard.isColliding(x + 0.98,y + 0.02)
        || gameboard.isColliding(x + 0.98,y + 0.98)) {

        x = oldX;
        y = oldY;
        didMovementCorrect = true;
    }
    else {
        didMovementCorrect = false;
    }

    // Make movement near walls feel better
    if (didMovementCorrect) {
        wallSlide();
        cornerSlip();
    }

    // Slow down for the next tick
    vx -= vx * deltaTime * 25;
    vy -= vy * deltaTime * 25;

    // Restart the movement clock for next delta time
    movementClock.restart();
}

// Slide along walls if needed
void Player::wallSlide() {
    if (vy < 0) {
        if (vx > 0) {
            if (!gameboard.isColliding(x + 1.05, y + 0.5)) {
                x += movementSpeed * deltaTime;
            }
        }
        if (vx < 0) {
            if (!gameboard.isColliding(x - 0.05, y + 0.5)) {
                x -= movementSpeed * deltaTime;
            }
        }
    }
    if (vy > 0) {
        if (vx > 0) {
            if (!gameboard.isColliding(x + 1.05, y + 0.5)) {
                x += movementSpeed * deltaTime;
            }
        }
        if (vx < 0) {
            if (!gameboard.isColliding(x - 0.05, y + 0.5)) {
                x -= movementSpeed * deltaTime;
            }
        }
    }
    if (vx < 0) {
        if (vy > 0) {
            if (!gameboard.isColliding(x + 0.5, y + 1.05)) {
                y += movementSpeed * deltaTime;
            }
        }
        if (vy < 0) {
            if (!gameboard.isColliding(x + 0.5, y - 0.05)) {
                y -= movementSpeed * deltaTime;
            }
        }
    }
    if (vx > 0) {
        if (vy > 0) {
            if (!gameboard.isColliding(x + 0.5, y + 1.05)) {
                y += movementSpeed * deltaTime;
            }
        }
        if (vy < 0) {
            if (!gameboard.isColliding(x + 0.5, y - 0.05)) {
                y -= movementSpeed * deltaTime;
            }
        }
    }
}

// Slide around corners if you just barely clip them
void Player::cornerSlip() {
    if (vy > 0) {

    }
    if (vy < 0) {

    }
    if (vx > 0) {

    }
    if (vx < 0) {

    }
}

// The player has died
void Player::die() {

}