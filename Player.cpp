//
// Created by fourteener on 11/1/19.
//

#include <cmath>

#include "Player.h"
#include "Weapon.h"
#include "Game.h"

// Blank player constructor for initialization purposes
Player::Player () {
    _x = -1, _y = -1, _vx = -1, _vy = -1, _movementSpeed = -1;
    _initialized = false;
};

// Regular player constructor
Player::Player(Board &board) {
    // Store the game board and get the size
    _gameboard = board;
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
    _x = initX;
    _y = initY;
    _vx = 0;
    _vy = 0;

    // Define movement speed
    _movementSpeed = 12;

    // Mark as initialized
    _initialized = true;
}

// Is the player initialized?
bool Player::isInitialized() const {
    return _initialized;
}

// Get the player's position
sf::Vector2f Player::getPos() const {
    return sf::Vector2f(_x * _gameboard.getTileSize(), _y * _gameboard.getTileSize());
}

// Move the player if the key is held by adding velocity
void Player::move(bool left, bool right, bool up, bool down) {
    // Used to scale the movement based on the time since last update
    _deltaTime = _movementClock.getElapsedTime().asMilliseconds() / 1000.0;

    // Move left/right, with resolution for moving both directions
    if (left && right)
        _vx = 0;
    else if (left)
        _vx = -left * _movementSpeed;
    else if (right)
        _vx = right * _movementSpeed;

    // Move up/down, with resolution for moving both directions
    if (up && down)
        _vy = 0;
    else if (up)
        _vy = -up * _movementSpeed;
    else if (down)
        _vy = down * _movementSpeed;

    // Prevents sqrt(2) speed boost when moving along a diagonal
    // This is because the vector (1,1) has length sqrt(2) rather than 1, so scale it down
    if ((left ||right) && (up || down)) {
        _vx /= sqrt(2);
        _vy /= sqrt(2);
    }

    // And tick player physics
    tickPhysics();
}

// Perform a physics tick
void Player::tickPhysics() {
    // Enforce a minimum speed so the player actually stops
    if (_vx * _vx < 1)
        _vx = 0;
    if (_vy * _vy < 1)
        _vy = 0;

    // Translate the player based on the velocity
    double oldX = _x;
    double oldY = _y;
    _x += _vx * _deltaTime;
    _y += _vy * _deltaTime;

    // Stop winding up in walls
    // AKA Poor man's collision detection
    // This pops the player out of walls if they moved into a wall during the last tick
    if (_gameboard.isColliding(_x + 0.02, _y + 0.02)
        || _gameboard.isColliding(_x + 0.02, _y + 0.98)
        || _gameboard.isColliding(_x + 0.98, _y + 0.02)
        || _gameboard.isColliding(_x + 0.98, _y + 0.98)) {

        _x = oldX;
        _y = oldY;

        // These make movement in close proximity to walls feel better
        // Either let a player slide along a wall they ran into or
        // Let the player slide around corners if they hit the wall close to a corner
        _wallSlide();
        _cornerSlip();
    }

    // Slow down for the next tick, preventing infinite movements
    // The factor of 25 makes movement not feel like ice physics
    // But just setting to 0 makes it feel too rigid
    _vx -= _vx * _deltaTime * 25;
    _vy -= _vy * _deltaTime * 25;

    // Restart the movement clock for next delta time
    _movementClock.restart();
}

// Slide along walls if needed
// This basically works by if the player ran into a wall their movement would be cancelled
// Along both axis, so this adds back the velocity back along the uncollided axis (after checking
// That the tile to slide into isn't also a wall
// This code is a lot of boilerplate for the system, and is almost identical as you go through
// But this method was about 10x faster than the best "compressed" method I could think of
void Player::_wallSlide() {
    if (_vy < 0) {
        if (_vx > 0) {
            if (!_gameboard.isColliding(_x + 1.05, _y + 0.5)) {
                _x += _movementSpeed * _deltaTime;
            }
        }
        if (_vx < 0) {
            if (!_gameboard.isColliding(_x - 0.05, _y + 0.5)) {
                _x -= _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vy > 0) {
        if (_vx > 0) {
            if (!_gameboard.isColliding(_x + 1.05, _y + 0.5)) {
                _x += _movementSpeed * _deltaTime;
            }
        }
        if (_vx < 0) {
            if (!_gameboard.isColliding(_x - 0.05, _y + 0.5)) {
                _x -= _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vx < 0) {
        if (_vy > 0) {
            if (!_gameboard.isColliding(_x + 0.5, _y + 1.05)) {
                _y += _movementSpeed * _deltaTime;
            }
        }
        if (_vy < 0) {
            if (!_gameboard.isColliding(_x + 0.5, _y - 0.05)) {
                _y -= _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vx > 0) {
        if (_vy > 0) {
            if (!_gameboard.isColliding(_x + 0.5, _y + 1.05)) {
                _y += _movementSpeed * _deltaTime;
            }
        }
        if (_vy < 0) {
            if (!_gameboard.isColliding(_x + 0.5, _y - 0.05)) {
                _y -= _movementSpeed * _deltaTime;
            }
        }
    }
}

// Slide around corners if you just barely clip them
// Basically if the player is pushing only one movement key and clips the side of a
// Corner, check if they can slide around it on either side and if so, let them slide around
// The corner, significantly improving movement within narrow diagonals and when near a wall corner
void Player::_cornerSlip() {
    if (_vy > 0) {
        if (_vx == 0) {
            if (!_gameboard.isColliding(_x - 0.45, _y + 1.1)) {
                _x -= _movementSpeed * _deltaTime;
            }
            if (!_gameboard.isColliding(_x + 1.45, _y + 1.1)) {
                _x += _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vy < 0) {
        if (_vx == 0) {
            if (!_gameboard.isColliding(_x - 0.45, _y - 0.1)) {
                _x -= _movementSpeed * _deltaTime;
            }
            if (!_gameboard.isColliding(_x + 1.45, _y - 0.1)) {
                _x += _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vx > 0) {
        if (_vy == 0) {
            if (!_gameboard.isColliding(_x + 1.1, _y - 0.45)) {
                _y -= _movementSpeed * _deltaTime;
            }
            if (!_gameboard.isColliding(_x + 1.1, _y + 1.45)) {
                _y += _movementSpeed * _deltaTime;
            }
        }
    }
    if (_vx < 0) {
        if (_vy == 0) {
            if (!_gameboard.isColliding(_x - 0.1, _y - 0.45)) {
                _y -= _movementSpeed * _deltaTime;
            }
            if (!_gameboard.isColliding(_x - 0.1, _y + 1.45)) {
                _y += _movementSpeed * _deltaTime;
            }
        }
    }
}

// Handles the logic for attacking
// Basically just spawns a bullet moving from the player toward the click location
void Player::attack(const sf::Vector2i mousePos) {
    // Create a weapon
    Weapon w (200, sf::Color::Red);
    w.setBounces(2);

    // Set needed variables
    double xPos, yPos, mouseX, mouseY, dirX, dirY;
    xPos = this->getPos().x + _gameboard.getTileSize() / 2.0;
    yPos = this->getPos().y + _gameboard.getTileSize() / 2.0;
    mouseX = mousePos.x;
    mouseY = mousePos.y;

    // Calculate the direction
    dirX = mouseX - xPos;
    dirY = mouseY - yPos;

    // Initialize the weapon
    w.init(xPos, yPos, dirX, dirY);

    // Spawn it into the world
    Game::getGame()->addWeapon(w);
}

// Check if a point is inside the player
// This is used for checking if something is colliding with the player, like say a bullet
bool Player::isColliding(double x, double y, const bool pixelPos) const {
    if (pixelPos) {
        x /= Game::getGame()->getTileSize();
        y /= Game::getGame()->getTileSize();
    }
    bool collision = false;
    if (x > _x + 0.15 && x < _x + 0.85 && y > _y + 0.15 && y < _y + 0.85) {
        collision = true;
    }
    return collision;
}

// Handles player death
void Player::die() {
    _dead = true;
}
 bool Player::isDead() const {
    return _dead;
}