//
// Created by fourteener on 10/31/19.
//

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

#include "Game.h"
#include "Board.h"
#include "Player.h"

// Did not want to type this a bunch
using std::vector;

// Have to initialize this or the static functions complain
// Using a nullptr means that if it doesn't get initialized (which is really hard); things will break and this is intentional
Game* Game::_theGame = nullptr;

// Return the game
Game* Game::getGame() {
    return _theGame;
}

// Return the player
Player* Game::getPlayer() {
    return &_player;
}

// Add a weapon object to the game
void Game::addWeapon(const Weapon &weapon) {
    _weapons.push_back(weapon);
}

// Add an enemy object to the game
void Game::addEnemy(const Enemy &enemy) {
    _enemies.push_back(enemy);
}

// Remove all enemies and weapons from the game
void Game::resetAddedObjects() {
    _weapons.clear();
    _enemies.clear();
}

// Returns the current level
int Game::getLevel() const {
    return _level;
}

// Returns board size
int Game::getNumTilesX() const {
    return _board.getBoard().size();
}
int Game::getNumTilesY() const {
    return _board.getBoard().at(0).size();
}

// Check board collisions
bool Game::boardCollision(const sf::Vector2f pos) const {
    return _board.isColliding(pos.x, pos.y);
}

// Gets the size of a tile
int Game::getTileSize() const {
    return _board.getTileSize();
}

// Get the best level
void Game::_getBestLevel() {
    // Load from the save file, 0 if it doesn't exist
    std::ifstream inFile("lvl.sav");
    if(!inFile) {
        _bestLevel = 0;
    }
    inFile >> _bestLevel;
    inFile.close();
}

// Update the best level to the file and variable; but only if needed
void Game::_updateBestLevel(int level) {
    // Load from the saved file the current value
    int saved = [](){
        std::ifstream inFile("lvl.sav");
        int savedBest = 0;
        if (!inFile) {
            savedBest = 0;
        }
        inFile >> savedBest;
        inFile.close();
        return savedBest;
    }();
    // Update it if the level value is better than the best level
    if (saved >= level)
        return;
    std::ofstream outFile("lvl.sav");
    if (!outFile) {}
    outFile << level;
    outFile.close();
}

// The central game loop
// This handles all the drawing of the game and all of the game logic (by instantiating and calling functions for other classes).
// Basically the main loop, but in a different file since it makes main.cpp really easy to read
// It also makes the game slightly more adherent to the principles of object oriented programming
void Game::run(const int windowSize, const int tileSize) {

    // Debug mode
    bool isDebug = true;

    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Wii Tanks Ghost Bullet Hell");
    window.setVerticalSyncEnabled(true);

    // Set the game
    _theGame = this;

    // Create a new board object and initialize it to start
    _board.blankBoard(windowSize, tileSize);
    _board.newBoard();

    // Has a game been played yet?
    bool hasPlayedGame = false;

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("game_resources/arial.ttf")) {
        // Well uhh this is bad
        // We really need that font to load
        return;
    }

    // Load some basic sprites
    sf::Texture wall, ghost, player;
    if (!wall.loadFromFile("game_resources/World.png")) {
        return;
    }
    if (!ghost.loadFromFile("game_resources/Ghost.png")) {
        return;
    }
    if (!player.loadFromFile("game_resources/Player.png")) {
        return;
    }

    // Load the best level
    _getBestLevel();

    // Key press deduplication variables
    bool keyNewMap = false;
    bool keyMovingLeft = false;
    bool keyMovingRight = false;
    bool keyMovingUp = false;
    bool keyMovingDown = false;

    // Rate limiting for clicks
    sf::Clock timeSinceLastClick;

    // Central game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handles events
            switch (event.type) {
                case sf::Event::Closed:
                    // Window closed
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    // If the user presses a key; see if anything needs to be done

                    // If pressed n, start regenerating the board
                    if (event.key.code == sf::Keyboard::N && !keyNewMap && !_gameRunning) {
                        _level = 1;
                        _board.newBoard();
                        _player = Player();
                        Player::health = 5;
                        keyNewMap = true;
                        _gameRunning = true;
                        hasPlayedGame = true;
                    }

                    // Start moving left
                    else if (event.key.code == sf::Keyboard::A && !keyMovingLeft) {
                        keyMovingLeft = true;
                    }

                    // Start moving right
                    else if (event.key.code == sf::Keyboard::D && !keyMovingRight) {
                        keyMovingRight = true;
                    }

                    // Start moving up
                    else if (event.key.code == sf::Keyboard::W && !keyMovingUp) {
                        keyMovingUp = true;
                    }

                    // Start moving down
                    else if (event.key.code == sf::Keyboard::S && !keyMovingDown) {
                        keyMovingDown = true;
                    }

                    // Debug tools
                    if (event.key.code == sf::Keyboard::H)
                        Player::health = 99999;
                    if (event.key.code == sf::Keyboard::L) {
                        _level += 10;
                        _board.newBoard();
                        _player = Player();
                        _gameRunning = true;
                    }
                    break;

                case sf::Event::KeyReleased:
                    // If the user releases a key; disable the key deduplication if needed
                    if (event.key.code == sf::Keyboard::N) {
                        keyNewMap = false;
                    }
                    else if (event.key.code == sf::Keyboard::A) {
                        keyMovingLeft = false;
                    }
                    else if (event.key.code == sf::Keyboard::D) {
                        keyMovingRight = false;
                    }
                    else if (event.key.code == sf::Keyboard::W) {
                        keyMovingUp = false;
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        keyMovingDown = false;
                    }
                    break;

                default:
                    // No event to handle so no need to do anything
                    break;
            }
        }

        // Game screen
        if (_gameRunning) {
            // Handle mouse inputs
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (timeSinceLastClick.getElapsedTime().asSeconds() >= 0.5) {
                    _player.attack(sf::Mouse::getPosition(window));
                    timeSinceLastClick.restart();
                }
            }

            // Poll the board for updates
            _board.pollBoard();
            if (!_player.isInitialized() && !_board.isGenerating()) {
                _player = Player(_board);
            }

            // Player movement update
            if (_player.isInitialized()) {
                _player.move(keyMovingLeft, keyMovingRight, keyMovingUp, keyMovingDown);
            }

            // Weapons movement update
            for (int i = 0; i < _weapons.size(); i++) {
                _weapons.at(i).move();
            }

            // Enemies movement update
            for (int i = 0; i < _enemies.size(); i++) {
                _enemies.at(i).move();
            }

            // Clear the current display
            window.clear(sf::Color::Black);

            // Get the board contents
            vector<vector<bool> > tiles = _board.getBoard();

            // Create the square array based on the board contents
            vector<sf::Sprite> tileShapes;
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles.at(i).size(); j++) {
                    if (tiles.at(i).at(j)) {
                        //sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));
                        //shape.setPosition(i * tileSize, j * tileSize);
                        //shape.setFillColor(sf::Color::Green);
                        sf::Sprite shape;
                        shape.setTexture(wall);
                        shape.setColor(sf::Color::Green);
                        shape.setPosition(i * tileSize, j * tileSize);
                        shape.setScale(getTileSize() / wall.getSize().x, getTileSize() / wall.getSize().y);
                        tileShapes.push_back(shape);
                    } else {
                        //sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));
                        //shape.setPosition(i * tileSize, j * tileSize);
                        //shape.setFillColor(sf::Color::Blue);
                        sf::Sprite shape;
                        shape.setTexture(wall);
                        shape.setColor(sf::Color::Blue);
                        shape.setPosition(i * tileSize, j * tileSize);
                        shape.setScale(getTileSize() / wall.getSize().x, getTileSize() / wall.getSize().y);
                        tileShapes.push_back(shape);
                    }
                }
            }

            // Draw the cave walls to the array
            for (sf::Sprite s : tileShapes) {
                window.draw(s);
            }

            // Check for weapons hitting things
            for (int j = 0; j < _weapons.size(); j++) {
                Weapon w = _weapons.at(j);
                double xLoc = w.getPosition().x;
                double yLoc = w.getPosition().y;
                if (!w.damaging())
                    continue;

                // Check player
                if (!w.isPlayerShot() && _player.isColliding(xLoc, yLoc, true)) {
                    _player.die();
                    _weapons.erase(_weapons.begin() + j);
                }

                // Check enemies
                if (!w.isPlayerShot())
                    continue;
                for (int i = 0; i < _enemies.size(); i++) {
                    if (_enemies.at(i).isColliding(xLoc, yLoc, true)) {
                        _enemies.at(i).die();
                        _weapons.erase(_weapons.begin() + j);
                        j--; // Prevent skipping a weapon
                    }
                }
            }

            // Check for a dead player and handle game over
            if (_player.isDead()) {
                _updateBestLevel(_level);
                _gameRunning = false;
            }

            // If there are no more enemies left, increment the level by 1
            if (_enemies.empty() && !_board.isGenerating()) {
                _level++;
                _board.newBoard();
                Player::health += ceil(_level / 3); // Heal the player a little bit each level, increasing as the game goes on
                _player = Player();
            }

            // Draw the player
            if (_player.isInitialized()) {
                sf::Vector2f playerPos = _player.getPos();
                sf::Sprite playerSprite;
                playerSprite.setTexture(player);
                playerSprite.setColor(sf::Color::Red);
                playerSprite.setPosition(playerPos);
                playerSprite.setScale(getTileSize() / player.getSize().x * 1.25, getTileSize() / player.getSize().y * 1.25);
                window.draw(playerSprite);
            }

            // Draw the weapons
            for (int i = 0; i < _weapons.size(); i++) {
                Weapon w = _weapons.at(i);
                if (!w.isAlive()) {
                    _weapons.erase(_weapons.begin() +
                                   i); // Might want to change weapons to a list rather than a vector since this is slow
                    i--; // This stops us from skipping over a weapon
                    continue;
                }
                sf::CircleShape cs;
                cs.setFillColor(w.getColor());
                cs.setRadius(3);
                cs.setPosition(w.getPosition());
                window.draw(cs);
            }

            // Draw the enemies
            for (int i = 0; i < _enemies.size(); i++) {
                Enemy e = _enemies.at(i);
                if (!e.isAlive()) {
                    _enemies.erase(_enemies.begin() + i);
                    i--; // Same as weapons, don't want to skip an update
                    continue;
                }
                sf::Sprite rs;
                rs.setTexture(ghost);
                rs.setColor(e.getColor());
                rs.setPosition(e.getPosition());
                rs.setScale(getTileSize() / ghost.getSize().x * 1.4, getTileSize() / ghost.getSize().y * 1.4);
                window.draw(rs);
            }

            // Draw some UI elements
            std::string text = " HP: " + std::to_string(Player::health);
            text += "\nLvl: " + std::to_string(_level);
            sf::Text textElement;
            textElement.setString(text);
            textElement.setStyle(sf::Text::Bold);
            textElement.setFont(font);
            textElement.setCharacterSize(30);
            textElement.setFillColor(sf::Color::Black);
            window.draw(textElement);

            // Draw the window
            window.display();
        }

        // Title screen
        else {
            // Clear the window
            window.clear(sf::Color::Black);

            // Draw some text with instructions and whatnot
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);

            // Draw text with instructions and whatnot
            std::string textToDraw;
            textToDraw += "Wii Tanks Ghost Bullet Hell";
            textToDraw += "\nUse WASD to move, click to shoot";
            textToDraw += "\nPress 'n' to begin a game";
            if (hasPlayedGame) {
                textToDraw += "\n\nYou made it to level " + std::to_string(_level);
            }
            else {
                textToDraw += "\n\n\n";
            }
            _getBestLevel();
            textToDraw += "\nBest level you've reached: " + std::to_string(_bestLevel);
            text.setString(textToDraw);
            window.draw(text);

            // Draw the window
            window.display();
        }
    }
}
