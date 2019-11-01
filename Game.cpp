//
// Created by fourteener on 10/31/19.
//

#include <SFML/Graphics.hpp>
#include <vector>

#include "Game.h"
#include "Board.h"

using std::vector;

// The central game loop
void Game::run(const int windowSize, const int tileSize) {

    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Tile-Based Cave");
    window.setVerticalSyncEnabled(true);

    // Create a new board object and initialize it to start
    Board board;
    board.blankBoard(windowSize, tileSize);

    // Key deduplication variables
    bool keyNewMap = false;

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
                        keyNewMap = true;
                    }
                    break;

                case sf::Event::KeyReleased:
                    // If the user releases a key; see if anything needs to be done

                    // If pressed n, start regenerating the board
                    if (event.key.code == sf::Keyboard::N) {
                        keyNewMap = false;
                    }
                    break;

                default:
                    // No event to handle so no need to do anything
                    break;
            }

        }

       // Poll the board for updates
       board.pollBoard();

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

        // Draw the window
        window.display();
    }
}
