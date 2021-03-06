//
// Created by fourteener on 11/1/19.
//

#ifndef SFML_PRACTICE_PLAYER_H
#define SFML_PRACTICE_PLAYER_H

#include <SFML/System.hpp>

#include "Board.h"

// Player class to allow the player to move
class Player {
public:
    // Constructors
    Player();
    explicit Player(Board& board);

    // Movement and collision physics functions
    void move(bool left, bool right, bool up, bool down);
    void tickPhysics();

    // Get if the player object exists and where it exists
    [[nodiscard]] bool isInitialized() const;
    [[nodiscard]] sf::Vector2f getPos() const;

    // Attack
    void attack(sf::Vector2i mousePos);

    // Check if a point inside the player, and a die function
    [[nodiscard]] bool isColliding(double x, double y, bool pixelPos) const;
    void die();
    [[nodiscard]] bool isDead() const;

    // The player should have a few health
    static int health;

private:
    // Helper functions to make movement near walls feel better
    void _wallSlide();
    void _cornerSlip();

    // Movement variables
    double _x, _y, _vx, _vy, _movementSpeed, _deltaTime;

    // Does this player actually exist?
    bool _initialized;

    // The game's gameboard
    Board _gameboard;

    // Used to compute deltaTime
    sf::Clock _movementClock;

    // Is the player dead?
    bool _dead = false;
};

#endif //SFML_PRACTICE_PLAYER_H
