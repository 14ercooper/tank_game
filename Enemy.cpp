//
// Created by fourteener on 11/23/19.
//

#include "Enemy.h"
#include "Weapon.h"
#include "Game.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>

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
    _clock.restart();
    _deltaClock.restart();
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
    sf::Vector2f movement = _getMovement();
    _xPos += movement.x * deltaTime * _movementSpeed;
    _yPos += movement.y * deltaTime * _movementSpeed;
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
        // TODO implement raycasting with wall bounces to be able to aim at the player using wall bounces
    }
    return (rand() % 36000) / 100.0;
}

// What direction to move?
sf::Vector2f Enemy::_getMovement() {
    if (_movementSpeed > 0.01) { // Don't do the math if it isn't needed
        // TODO implement pathfinding algorithm
    }
    return sf::Vector2f(0,0);
}