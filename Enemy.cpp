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
Enemy::Enemy(double attackRate, double weaponSpeed, int weaponBounces, sf::Color color, double xPos, double yPos) {
    _attackRate = attackRate;
    _weaponSpeed = weaponSpeed;
    _weaponBounces = weaponBounces;
    _color = color;
    _xPos = xPos * Game::getGame()->getTileSize();
    _yPos = yPos * Game::getGame()->getTileSize();
    _isAlive = true;
    _clock.restart();
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
    if (_clock.getElapsedTime().asSeconds() >= _attackRate) {
        _attack((rand() % 36000) / 100.0);
        _clock.restart();
    }
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
    std::cout << x << ";" << _xPos << "\n";
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