//
// Created by fourteener on 11/23/19.
//

#ifndef SFML_PRACTICE_ENEMY_H
#define SFML_PRACTICE_ENEMY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Enemy {
public:
    Enemy(double attackRate, double weaponSpeed, int weaponBounces, sf::Color color, double xPos, double yPos);
    void move();
    sf::Color getColor();
    sf::Vector2f getPosition();
    bool isAlive();

    bool isColliding(double x, double y, bool pixelPos);
    void die();

private:
    Enemy();
    void _attack(double angle);
    double _attackRate, _weaponSpeed, _xPos, _yPos;
    int _weaponBounces;
    bool _isAlive;
    sf::Color _color;
    sf::Clock _clock;
};

#endif //SFML_PRACTICE_ENEMY_H
