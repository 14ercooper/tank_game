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
    Enemy(double attackRate, double weaponSpeed, int weaponBounces, sf::Color color, double xPos, double yPos, double movementSpeed, bool smartAim);

    // Basically trigger the AI
    void move();

    // Getters for different variables
    [[nodiscard]] sf::Color getColor() const ;
    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] bool isAlive() const;

    // Check collision and allow the enemy to die
    [[nodiscard]] bool isColliding(double x, double y, bool pixelPos) const;
    void die();

private:
    // Hide this since who knows what it would cause to happen
    Enemy();

    // Attack/spawn a weapon
    void _attack(double angle) const;

    // Get attack angle
    [[nodiscard]] double _aimAtPlayer() const;

    // Get movement direction
    void _getMovement();

    // Enemy variables
    double _attackRate{}, _weaponSpeed{}, _xPos{}, _yPos{}, _movementSpeed{};
    int _weaponBounces{};
    bool _isAlive{}, _smartAim{};
    double _weaponDelay{};
    double _speedMult = 1.0;
    sf::Color _color;

    // Used as an attack timer
    sf::Clock _clock;

    // Used to compute deltaTime
    sf::Clock _deltaClock;

    // Don't calculate movement too often
    sf::Clock _movementClock;
    double _movementOffset;
    bool _strafeClockwise;
    sf::Vector2f _movement;
};

#endif //SFML_PRACTICE_ENEMY_H
