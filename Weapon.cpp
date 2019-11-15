//
// Created by fourteener on 11/6/19.
//

#include <cmath>
#include <iostream>

#include "Weapon.h"
#include "Game.h"

// Blank constructor
Weapon::Weapon() {
    _alive = false;
    _posX = 0;
    _posY = 0;
    _dirX = 0;
    _dirY = 0;
    _color = sf::Color::Black;
    _speed = 0;
    _maxBounces = 0;
    _bouncesDone = 0;
}

// Constructor to set speed and color
Weapon::Weapon(double speed, sf::Color color) {
    this->_speed = speed;
    this->_color = color;
    _posX = 0;
    _posY = 0;
    _dirX = 0;
    _dirY = 0;
    _maxBounces = 0;
    _bouncesDone = 0;
    _alive = false;
}

// Return the color of the weapon
sf::Color Weapon::getColor() {
    return _color;
}

// Is this weapon alive?
bool Weapon::isAlive() {
    return _alive;
}

// Returns the current position of this weapon
sf::Vector2f Weapon::getPosition() {
    return sf::Vector2f(_posX, _posY);
}

// How many times can this weapon bounce
void Weapon::setBounces(int bounces) {
    _maxBounces = bounces;
}

// Initialize the weapon to be part of the game world
// Note that this does not spawn it into the game
void Weapon::init(double posX, double posY, double dirX, double dirY) {
    // Sets position
    _posX = posX;
    _posY = posY;

    // Sets direction of movement
    _dirX = dirX;
    _dirY = dirY;

    // Normalize  the direction vector
    double mag = sqrt((_dirX * _dirX) + (_dirY * _dirY));
    _dirX /= mag;
    _dirY /= mag;

    // Marks this weapon as alive
    _alive = true;

    // Make sure deltaTime works
    _clock.restart();
}

// Move the weapon
void Weapon::move() {

    // Used to scale speed based on elapsed time
    double deltaTime = _clock.getElapsedTime().asMilliseconds() / 1000.0;

    // Move the weapon, with speed scaled by deltaTime
    _posX += _dirX * _speed * deltaTime;
    _posY += _dirY * _speed * deltaTime;

    // Reset the clock to make sure deltaTime is correct
    _clock.restart();

    // Handle wall collisions
    _isInWall();
}

// Deal with wall collisions
void Weapon::_isInWall() {
    bool wallLeft = Game::getGame()->boardCollision(sf::Vector2f((_posX - 1) / Game::getGame()->getTileSize(), (_posY + 3) / Game::getGame()->getTileSize()));
    bool wallRight = Game::getGame()->boardCollision(sf::Vector2f((_posX + 7) / Game::getGame()->getTileSize(), (_posY + 3) / Game::getGame()->getTileSize()));
    bool wallUp = Game::getGame()->boardCollision(sf::Vector2f((_posX + 3) / Game::getGame()->getTileSize(), (_posY - 1) / Game::getGame()->getTileSize()));
    bool wallDown = Game::getGame()->boardCollision(sf::Vector2f((_posX + 3) / Game::getGame()->getTileSize(), (_posY + 7) / Game::getGame()->getTileSize()));

    // If the attack is in a wall, bounce if possible else despawn
    if (wallLeft || wallRight || wallUp || wallDown) {
        // Despawn (no more bounces)
        if (_bouncesDone >= _maxBounces) {
            _alive = false;
            return;
        }

        // Handle bouncing
        if (wallLeft || wallRight) {
            _dirX *= -1;
            _bouncesDone++;
            _posX += _dirX;
        }
        if (wallUp || wallDown) {
            _dirY *= -1;
            _bouncesDone++;
            _posY += _dirY;
        }
    }
}