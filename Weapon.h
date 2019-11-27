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
    [[nodiscard]] sf::Color getColor() const;

    // Is this weapon alive
    [[nodiscard]] bool isAlive() const;

    // Get the position of this weapon
    [[nodiscard]] sf::Vector2f getPosition() const;

    // Who shot it?
    [[nodiscard]] bool isPlayerShot();

    // Initialize the weapon instance, making it ready for spawning
    void init(double posX, double posY, double dirX, double dirY, bool playerShot);

    // Physics tick for weapon objects
    void move();

    // Set how many times the weapon can bounce off walls
    void setBounces(int bounces);

    // Can this deal damage?
    [[nodiscard]] bool damaging() const;

private:
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

    // This clock is the time the weapon has existed
    sf::Clock _aliveTime;

    // Variables to help with bouncing
    int _maxBounces, _bouncesDone;

    // Prevent friendly fire
    bool _playerShot;
};

#endif //SFML_PRACTICE_WEAPON_H
