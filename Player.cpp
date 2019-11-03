//
// Created by fourteener on 11/1/19.
//

#include <cmath>

#include "Player.h"

// Blank player constructor for initialization purposes
Player::Player () {
    x = -1, y = -1, vx = -1, vy = -1, movementSpeed = -1;
    initialized = false;
};

// Regular player constructor
Player::Player(Board &board) {
    // Store the game board and get the size
    gameboard = board;
    const int mapSize = board.getBoard().size();
    double initX, initY;
    // Get an air tile to spawn the player on
    while (true) {
        initX = (rand() % mapSize);
        initY = (rand() % mapSize);
        if (!board.isColliding(initX, initY)) {
            break;
        }
    }

    // Define initial position and velocity of the player
    x = initX;
    y = initY;
    vx = 0;
    vy = 0;

    // Define movement speed
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
    // Used to scale the movement based on the time since last update
    deltaTime = movementClock.getElapsedTime().asMilliseconds() / 1000.0;

    // Move left/right, with resolution for moving both directions
    if (left && right)
        vx = 0;
    else if (left)
        vx = -left * movementSpeed;
    else if (right)
        vx = right * movementSpeed;

    // Move up/down, with resolution for moving both directions
    if (up && down)
        vy = 0;
    else if (up)
        vy = -up * movementSpeed;
    else if (down)
        vy = down * movementSpeed;

    // Prevents sqrt(2) speed boost when moving along a diagonal
    // This is because the vector (1,1) has length sqrt(2) rather than 1, so scale it down
    if ((left ||right) && (up || down)) {
        vx /= sqrt(2);
        vy /= sqrt(2);
    }

    // And tick player physics
    tickPhysics();
}

// Perform a physics tick
void Player::tickPhysics() {
    // Enforce a minimum speed so the player actually stops
    if (vx * vx < 1)
        vx = 0;
    if (vy * vy < 1)
        vy = 0;

    // Translate the player based on the velocity
    double oldX = x;
    double oldY = y;
    x += vx * deltaTime;
    y += vy * deltaTime;

    // Stop winding up in walls
    // AKA Poor man's collision detection
    // This pops the player out of walls if they moved into a wall during the last tick
    if (gameboard.isColliding(x + 0.02,y + 0.02)
        || gameboard.isColliding(x + 0.02,y + 0.98)
        || gameboard.isColliding(x + 0.98,y + 0.02)
        || gameboard.isColliding(x + 0.98,y + 0.98)) {

        x = oldX;
        y = oldY;

        // These make movement in close proximity to walls feel better
        // Either let a player slide along a wall they ran into or
        // Let the player slide around corners if they hit the wall close to a corner
        wallSlide();
        cornerSlip();
    }

    // Slow down for the next tick, preventing infinite movements
    // The factor of 25 makes movement not feel like ice physics
    // But just setting to 0 makes it feel too rigid
    vx -= vx * deltaTime * 25;
    vy -= vy * deltaTime * 25;

    // Restart the movement clock for next delta time
    movementClock.restart();
}

// Slide along walls if needed
// This basically works by if the player ran into a wall their movement would be cancelled
// Along both axis, so this adds back the velocity back along the uncollided axis (after checking
// That the tile to slide into isn't also a wall
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
// Basically if the player is pushing only one movement key and clips the side of a
// Corner, check if they can slide around it on either side and if so, let them slide around
// The corner, significantly improving movement within narrow diagonals and when near a wall corner
void Player::cornerSlip() {
    if (vy > 0) {
        if (vx == 0) {
            if (!gameboard.isColliding(x - 0.45, y + 1.1)) {
                x -= movementSpeed * deltaTime;
            }
            if (!gameboard.isColliding(x + 1.45, y + 1.1)) {
                x += movementSpeed * deltaTime;
            }
        }
    }
    if (vy < 0) {
        if (vx == 0) {
            if (!gameboard.isColliding(x - 0.45, y - 0.1)) {
                x -= movementSpeed * deltaTime;
            }
            if (!gameboard.isColliding(x + 1.45, y - 0.1)) {
                x += movementSpeed * deltaTime;
            }
        }
    }
    if (vx > 0) {
        if (vy == 0) {
            if (!gameboard.isColliding(x + 1.1, y - 0.45)) {
                y -= movementSpeed * deltaTime;
            }
            if (!gameboard.isColliding(x + 1.1, y + 1.45)) {
                y += movementSpeed * deltaTime;
            }
        }
    }
    if (vx < 0) {
        if (vy == 0) {
            if (!gameboard.isColliding(x - 0.1, y - 0.45)) {
                y -= movementSpeed * deltaTime;
            }
            if (!gameboard.isColliding(x - 0.1, y + 1.45)) {
                y += movementSpeed * deltaTime;
            }
        }
    }
}