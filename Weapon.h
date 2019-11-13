//
// Created by fourteener on 11/6/19.
//

#ifndef SFML_PRACTICE_WEAPON_H
#define SFML_PRACTICE_WEAPON_H

#include <SFML/Graphics.hpp>

class Weapon {
public:
    // Constructors
    Weapon();
    Weapon(double speed, sf::Color color);

    // What color is this weapon?
    sf::Color getColor();

    // Is this weapon alive
    bool isAlive();

    // Get the position of this weapon
    sf::Vector2f getPosition();

    // Initialize the weapon instance, making it ready for spawning
    void init(double posX, double posY, double dirX, double dirY);

    // Physics tick for weapon objects
    void move();

protected:
    // How fast is the weapon?
    double _speed;

    // Position and movement of this weapon
    double _posX, _posY, _dirX, _dirY;

    // Check for wall collisions
    void _isInWall();

    // Stores if this weapon is alive
    bool _alive;

    // Stores the color of this weapon
    sf::Color _color;

    // This clock is used to compute deltaTime
    sf::Clock _clock;
};

#endif //SFML_PRACTICE_WEAPON_H
