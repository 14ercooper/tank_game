//
// Created by fourteener on 11/23/19.
//

#include "Enemy.h"
#include "Weapon.h"
#include "Game.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>

// Create a new enemy based on these attributes
Enemy::Enemy(const double attackRate, const double weaponSpeed, const int weaponBounces, const sf::Color color, const double xPos, const double yPos, const double movementSpeed, const bool smartAim) {
    _attackRate = attackRate;
    _weaponSpeed = weaponSpeed;
    _weaponDelay = attackRate * ((rand() % 1000) / 1000.0);
    _weaponBounces = weaponBounces;
    _color = color;
    _xPos = xPos * Game::getGame()->getTileSize();
    _yPos = yPos * Game::getGame()->getTileSize();
    _isAlive = true;
    _movementSpeed = movementSpeed;
    _smartAim = smartAim;
    _movementOffset = 0.5 * ((rand() % 1000) / 1000.0);
    _strafeClockwise = rand() % 2;
    _clock.restart();
    _deltaClock.restart();
    _movementClock.restart();
}

// Default constructor hidden and empty
// Nothing uses it, make sure nothing uses it
Enemy::Enemy() = default;

// Get the enemy color
sf::Color Enemy::getColor() const {
    return _color;
}

// Get the enemy location
sf::Vector2f Enemy::getPosition() const {
    return sf::Vector2f(_xPos, _yPos);
}

// Is the enemy still alive?
bool Enemy::isAlive() const {
    return _isAlive;
}

// Tick the AI
// This does stuff like moving and attacking
void Enemy::move() {
    // Attack when needed
    if (_clock.getElapsedTime().asSeconds() >= _attackRate + _weaponDelay) {
        _attack(_aimAtPlayer());
        _weaponDelay = 0;
        _clock.restart();
    }

    // Move
    double deltaTime = _deltaClock.getElapsedTime().asSeconds();
    _getMovement();
    _xPos += _movement.x * deltaTime * _movementSpeed * _speedMult;
    _yPos += _movement.y * deltaTime * _movementSpeed * _speedMult;
    _deltaClock.restart();
}

// Attack function
void Enemy::_attack(const double angle) const {
    // Create a weapon
    Weapon w (_weaponSpeed, sf::Color::Red);
    w.setBounces(_weaponBounces);

    // Calculate the direction
    double dirX = cos(angle);
    double dirY = sin(angle);

    // Initialize the weapon
    w.init(_xPos, _yPos, dirX, dirY);

    // Get it into the game world
    Game::getGame()->addWeapon(w);
}

// Check if a point is inside the enemy
// This is more or less the same collision code as the player has, but has a toggle for the two coordinate systems
bool Enemy::isColliding(double x, double y, const bool pixelPos) const {
    int tileSize = Game::getGame()->getTileSize();
    if (!pixelPos) {
        x *= tileSize;
        y *= tileSize;
    }
    bool collision = false;
    if (x > _xPos - (0.1 * tileSize) && x < _xPos + (1.1 * tileSize) && y > _yPos - (0.1 * tileSize) && y < _yPos + (1.1 * tileSize)) {
        collision = true;
    }
    return collision;
}

// Die
void Enemy::die() {
    _isAlive = false;
}

// Get the direction to shoot to hit the player
// First checks at 1 degree increments around the circle for the shot that gets closest to the player
// Then refines the shot to be accurate to the hundredth of a degree, giving the enemies very good aim if the player is in a hitabble area
// This also accounts for bouncing off walls, leading to the enemy being capable of some pretty sweet trick shots from behind walls and across the map
// I feel kinda bad for the player, with enemies this accurate
double Enemy::_aimAtPlayer() const {
    if (_smartAim) { // Don't do the math if it isn't needed
        Game* game = Game::getGame();
        sf::Vector2f playerPos = game->getPlayer()->getPos();
        playerPos.x += game->getTileSize() / 2;
        playerPos.y += game->getTileSize() / 2;

        // Test 360 angles to see which is the best
        double bestAngle = -1;
        double bestPlayerDist = 99999999;
        for (int angle = 0; angle < 360; angle++) {
            // Simulate a bullet
            int bouncesUsed = 0;
            double dirX = cos(angle * 1.0); // Make sure these are doubles
            double dirY = sin(angle * 1.0);
            double posX = _xPos;
            double posY = _yPos;
            double minPlayerDist = 99999999;
            while (bouncesUsed <= _weaponBounces) {
                posX += dirX * 2;
                posY += dirY * 2;

                // Wall bouncing
                bool wallLeft = Game::getGame()->boardCollision(sf::Vector2f((posX - 1) / Game::getGame()->getTileSize(), (posY + 3) / Game::getGame()->getTileSize()));
                bool wallRight = Game::getGame()->boardCollision(sf::Vector2f((posX + 7) / Game::getGame()->getTileSize(), (posY + 3) / Game::getGame()->getTileSize()));
                bool wallUp = Game::getGame()->boardCollision(sf::Vector2f((posX + 3) / Game::getGame()->getTileSize(), (posY - 1) / Game::getGame()->getTileSize()));
                bool wallDown = Game::getGame()->boardCollision(sf::Vector2f((posX + 3) / Game::getGame()->getTileSize(), (posY + 7) / Game::getGame()->getTileSize()));
                if (wallLeft || wallRight) {
                    dirX *= -1;
                    bouncesUsed++;
                    posX += dirX;
                }
                if (wallUp || wallDown) {
                    dirY *= -1;
                    bouncesUsed++;
                    posY += dirY;
                }

                // Update distance to player
                double playerDist = pow(posX - playerPos.x, 2) + pow(posY - playerPos.y, 2);
                if (playerDist < minPlayerDist)
                    minPlayerDist = playerDist;
            }
            // Update best angle if needed
            if (minPlayerDist < bestPlayerDist) {
                bestPlayerDist = minPlayerDist;
                bestAngle = angle;
            }
        }

        // Really hone in on sniping the player
        // More or less the same as the initial tests, just trying to get even more accurate
        // Some parts could be a reused function, but enough was different that I kept it out of a function
        double bestAngleDecimal = -1;
        bestPlayerDist = 99999999;
        for (int angle = -50; angle < 50; angle++) {
            // Simulate a bullet
            int bouncesUsed = 0;
            double dirX = cos((bestAngle + (angle / 100.0)) * 1.0); // Make sure these are doubles
            double dirY = sin((bestAngle + (angle / 100.0)) * 1.0);
            double posX = _xPos;
            double posY = _yPos;
            double minPlayerDist = 99999999;
            while (bouncesUsed <= _weaponBounces) {
                posX += dirX * 2;
                posY += dirY * 2;

                // Wall bouncing
                bool wallLeft = Game::getGame()->boardCollision(sf::Vector2f((posX - 1) / Game::getGame()->getTileSize(), (posY + 3) / Game::getGame()->getTileSize()));
                bool wallRight = Game::getGame()->boardCollision(sf::Vector2f((posX + 7) / Game::getGame()->getTileSize(), (posY + 3) / Game::getGame()->getTileSize()));
                bool wallUp = Game::getGame()->boardCollision(sf::Vector2f((posX + 3) / Game::getGame()->getTileSize(), (posY - 1) / Game::getGame()->getTileSize()));
                bool wallDown = Game::getGame()->boardCollision(sf::Vector2f((posX + 3) / Game::getGame()->getTileSize(), (posY + 7) / Game::getGame()->getTileSize()));
                if (wallLeft || wallRight) {
                    dirX *= -1;
                    bouncesUsed++;
                    posX += dirX;
                }
                if (wallUp || wallDown) {
                    dirY *= -1;
                    bouncesUsed++;
                    posY += dirY;
                }

                // Update distance to player
                double playerDist = pow(posX - playerPos.x, 2) + pow(posY - playerPos.y, 2);
                if (playerDist < minPlayerDist)
                    minPlayerDist = playerDist;
            }
            // Update best angle if needed
            if (minPlayerDist < bestPlayerDist) {
                bestPlayerDist = minPlayerDist;
                bestAngleDecimal = angle;
            }
        }

        return bestAngle + (bestAngleDecimal / 100.0);
    }
    // If it doesn't use smart aim, shoot randomly
    return (rand() % 36000) / 100.0;
}

// What direction to move?
// Goal is to keep 8 to 16 units away from the player at all times
void Enemy::_getMovement() {
    if (_movementSpeed > 0.01) { // Don't do the math if it isn't needed
        // Start by not recalculating the path every frame
        if (_movementClock.getElapsedTime().asSeconds() > 0.5 + _movementOffset) {
            _movementClock.restart();

            // Grab the player location and board reference
            Game* game = Game::getGame();
            sf::Vector2f playerPos = game->getPlayer()->getPos();
            double thisX = _xPos;
            double thisY = _yPos;
            double distToPlayer = sqrt(pow(thisX - playerPos.x, 2) + pow(thisY - playerPos.y, 2));

            // Case 1: too close to player so "panic" and run directly away
            if (distToPlayer < 8.0 * game->getTileSize()) {
                // Return the direction to run in (if possible, otherwise it'll bounce down to the next case
                sf::Vector2f fleeDir;
                fleeDir.x = thisX - playerPos.x;
                fleeDir.y = thisY - playerPos.y;
                fleeDir.x /= sqrt(pow(fleeDir.x, 2) + pow(fleeDir.y, 2)); // Normalize vector
                fleeDir.y /= sqrt(pow(fleeDir.x, 2) + pow(fleeDir.y, 2));

                // Avoid moving into a wall
                _speedMult = 1.5;
                _movement = fleeDir;
                return;
            }

            // Case 2: Too far from player so run toward them
            else if (distToPlayer > 16.0 * game->getTileSize()) {
                // Move toward the player
                sf::Vector2f moveDir;
                moveDir.x = playerPos.x - thisX;
                moveDir.y = playerPos.y - thisY;
                moveDir.x /= sqrt(pow(moveDir.x, 2) + pow(moveDir.y, 2)); // Normalize vector
                moveDir.y /= sqrt(pow(moveDir.x, 2) + pow(moveDir.y, 2));

                // Avoid moving into a wall
                _movement = moveDir;
                return;
            }

            // Case 3: Good distance from player so stay put
            else {
                _movement = sf::Vector2f(0,0);
                return;
            }
        }
        return;
    }
    // If they aren't supposed to move, just make them not move
    _movement = sf::Vector2f(0,0);
}
