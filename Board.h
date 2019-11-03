//
// Created by fourteener on 10/31/19.
//

#ifndef SFML_PRACTICE_BOARD_H
#define SFML_PRACTICE_BOARD_H

#include <vector>
#include <SFML/System.hpp>

// The gameboard class
class Board {
public:
    // Causes the gameboard to perform any waiting actions
    void pollBoard();

    // Create a blank board based on the parameters
    void blankBoard(int windowSize, int tileSize);

    // Populate the board and begin the generation process
    void newBoard();

    // Poll the current state of the board
    bool isGenerating();

    // Checks if a wall is occupying the given location
    bool isColliding (double x, double y);

    // Returns a reference to this board
    std::vector< std::vector<bool> > getBoard();

    // Gets the size of a tile on this board
    int getTileSize();

private:
    // Actually stores the board
    std::vector< std::vector<bool> > tiles;

    // Keeps track of where in the generation process the board is
    int iterationsDone;
    int iterType;

    // Used to be able to slow down the generation (debugging or special effects)
    sf::Clock clock;

    // Is the board being built/is it generating?
    bool buildingBoard;

    // The size of a tile
    int tileSize;

    // If the gameboard is too small/has too little air, lets the board get regenerated
    bool regenerate;

    // Fills the board randomly with walls and air
    void fillBoard();

    // Performs a generation iteration
    void iterate();

    // Is there a wall here?
    bool isWall(int x, int y);

    // Gets the number of walls with steps of the wall
    // 8-directional steps (diagonal counts as a step as well)
    int getNearbyWalls(int x, int y, int steps);

    // Makes sure the board is surrounded by walls
    void fillWalls();

    // Picks a random air tile, and fills every air not connected with wall
    // This removes disconnected caverns
    // Picking randomly usually chooses the largest section
    // Otherwise a regneration is triggered, but this is rare and doesn't come near the maximum acceptable genration time
    void floodFill();

    // Cave generation constants, these determine the form of the playing field
    // They are the cellular automata parameters
    int firstIters;
    int secondIters;
    double startChance;
    int makeWall;
    int bigArea;
    double airPercent;
};

#endif //SFML_PRACTICE_BOARD_H
