//
// Created by fourteener on 11/6/19.
//

#include "Weapon.h"

// Blank constructor
Weapon::Weapon() {

}

// Constructor to set speed
Weapon::Weapon(double speed) {
    this->_speed = speed;
}

// Return the color of the weapon
sf::Color Weapon::getColor() {
    return _color;
}

void Weapon::spawn() {

}

void Weapon::spawn(double posX, double posY, double dirX, double dirY) {

}