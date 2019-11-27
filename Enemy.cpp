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
Enemy::Enemy(double attackRate, double weaponSpeed, int weaponBounces, sf::Color color, double xPos, double yPos, double movementSpeed, bool smartAim) {
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
Enemy::Enemy() {}

// Get the enemy color
sf::Color Enemy::getColor() {
    return _color;
}

// Get the enemy location
sf::Vector2f Enemy::getPosition() {
    return sf::Vector2f(_xPos, _yPos);
}

// Is the enemy still alive?
bool Enemy::isAlive() {
    return _isAlive;
}

// Tick the AI
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
void Enemy::_attack(double angle) {
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

// Check if a point is inside the player
bool Enemy::isColliding(double x, double y, bool pixelPos) {
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
double Enemy::_aimAtPlayer() {
    if (_smartAim) { // Don't do the math if it isn't needed
        Game* game = Game::getGame();
        sf::Vector2f playerPos = game->getPlayer()->getPos();

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
            int minPlayerDist = 99999999;
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
        double bestAngleDecimal = -1;
        bestPlayerDist = 99999999;
        for (int angle = -50; angle < 50; angle++) {
            // Simulate a bullet
            int bouncesUsed = 0;
            double dirX = cos((bestAngle + (angle / 100.0)) * 1.0); // Make sure these are doubles
            double dirY = sin((bestAngle + (angle / 100.0)) * 1.0);
            double posX = _xPos;
            double posY = _yPos;
            int minPlayerDist = 99999999;
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
    _movement = sf::Vector2f(0,0);
}
