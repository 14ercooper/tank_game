//
// Created by fourteener on 11/6/19.
//

#ifndef SFML_PRACTICE_WEAPON_H
#define SFML_PRACTICE_WEAPON_H

#include <SFML/Graphics.hpp>

class Weapon {
public:
    Weapon();
    Weapon(double speed);
    sf::Color getColor();
    void spawn();
    void spawn(double posX, double posY, double dirX, double dirY);
    virtual void move() = 0;
protected:
    double _speed;
    double _posX, _posY, _dirX, _dirY;
    sf::Color _color;
};

#endif //SFML_PRACTICE_WEAPON_H
