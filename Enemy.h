//
// Created by fourteener on 11/23/19.
//

#ifndef SFML_PRACTICE_ENEMY_H
#define SFML_PRACTICE_ENEMY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Enemy {
public:
    // Create a new enemy of the stupid variety
    Enemy(double attackRate, double weaponSpeed, int weaponBounces, sf::Color color, double xPos, double yPos);

    // Basically trigger the AI
    void move();

    // Getters for different variables
    sf::Color getColor();
    sf::Vector2f getPosition();
    bool isAlive();

    // Check collision and allow the enemy to die
    bool isColliding(double x, double y, bool pixelPos);
    void die();

private:
    // Hide this since who knows what it would cause to happen
    Enemy();

    // Attack/spawn a weapon
    void _attack(double angle);

    // Enemy variables
    double _attackRate, _weaponSpeed, _xPos, _yPos;
    int _weaponBounces;
    bool _isAlive;
    sf::Color _color;

    // Used as an attack timer
    sf::Clock _clock;
};

#endif //SFML_PRACTICE_ENEMY_H
