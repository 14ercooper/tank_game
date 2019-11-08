//
// Created by fourteener on 10/31/19.
//

#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.h"
#include "Board.h"
#include "Player.h"

using std::vector;

Game* Game::_theGame = nullptr;

// Return the game
Game* Game::getGame() {
    return _theGame;
}

// Add a weapon object to the game
void Game::addWeapon(Weapon &weapon) {
    weapons.push_back(weapon);
}

// The central game loop
void Game::run(const int windowSize, const int tileSize) {

    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Tile-Based Cave");
    window.setVerticalSyncEnabled(true);

    // Set the game
    _theGame = this;

    // Create a new board object and initialize it to start
    Board board;
    board.blankBoard(windowSize, tileSize);
    board.newBoard();

    // Create a player object
    Player player;

    // Key deduplication variables
    bool keyNewMap = false;
    bool keyMovingLeft = false;
    bool keyMovingRight = false;
    bool keyMovingUp = false;
    bool keyMovingDown = false;

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
                    if (event.key.code == sf::Keyboard::N && !keyNewMap) {
                        board.newBoard();
                        player = Player();
                        keyNewMap = true;
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

       // Poll the board for updates
       board.pollBoard();
        if (!player.isInitialized() && !board.isGenerating()) {
            player = Player(board);
        }

        // Player movement update
        if (player.isInitialized()) {
            player.move(keyMovingLeft, keyMovingRight, keyMovingUp, keyMovingDown);
        }

        // Weapons movement update
        for (Weapon w : weapons) {
            w.move();
        }

        // Clear the current display
        window.clear(sf::Color::Black);

        // Get the board contents
        vector< vector<bool> > tiles = board.getBoard();

        // Create the square array based on the board contents
        vector<sf::RectangleShape> tileShapes;
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles.at(i).size(); j++) {
                if (tiles.at(i).at(j)) {
                    sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));
                    shape.setPosition(i * tileSize, j * tileSize);
                    shape.setFillColor(sf::Color::Green);
                    tileShapes.push_back(shape);
                }
                else {
                    sf::RectangleShape shape(sf::Vector2f(tileSize, tileSize));
                    shape.setPosition(i * tileSize, j * tileSize);
                    shape.setFillColor(sf::Color::Blue);
                    tileShapes.push_back(shape);
                }
            }
        }

        // Draw the cave walls to the array
        for (sf::RectangleShape s : tileShapes) {
            window.draw(s);
        }

        // Draw the player
        if (player.isInitialized()) {
            sf::Vector2f playerPos = player.getPos();
            sf::RectangleShape player (sf::Vector2f(tileSize, tileSize));
            player.setPosition(playerPos);
            player.setFillColor(sf::Color::Red);
            window.draw(player);
        }


        // Draw the window
        window.display();
    }
}