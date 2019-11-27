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
    [[nodiscard]] bool isGenerating() const;

    // Checks if a wall is occupying the given location
    [[nodiscard]] bool isColliding (double x, double y) const;

    // Returns a reference to this board
    [[nodiscard]] std::vector< std::vector<bool> > getBoard() const;

    // Gets the size of a tile on this board
    [[nodiscard]] int getTileSize() const;

private:
    // Actually stores the board
    std::vector< std::vector<bool> > _tiles;

    // Keeps track of where in the generation process the board is
    int _iterationsDone;
    int _iterType;

    // Used to be able to slow down the generation (debugging or special effects)
    sf::Clock _clock;

    // Is the board being built/is it generating?
    bool _buildingBoard;

    // The size of a tile
    int _tileSize;

    // If the gameboard is too small/has too little air, lets the board get regenerated
    bool _regenerate;

    // Fills the board randomly with walls and air
    void _fillBoard();

    // Performs a generation iteration
    void _iterate();

    // Is there a wall here?
    [[nodiscard]] bool _isWall(int x, int y) const;

    // Gets the number of walls with steps of the wall
    // 8-directional steps (diagonal counts as a step as well)
    [[nodiscard]] int _getNearbyWalls(int x, int y, int steps) const;

    // Makes sure the board is surrounded by walls
    void _fillWalls();

    // Picks a random air tile, and fills every air not connected with wall
    // This removes disconnected caverns
    // Picking randomly usually chooses the largest section
    // Otherwise a regneration is triggered, but this is rare and doesn't come near the maximum acceptable genration time
    void _floodFill();

    // Handles the logic for spawning enemies into the game
    void _spawnEnemy(int &points);

    // Cave generation constants, these determine the form of the playing field
    // They are the cellular automata parameters
    int _firstIters;
    int _secondIters;
    double _startChance;
    int _makeWall;
    int _bigArea;
    double _airPercent;
};

#endif //SFML_PRACTICE_BOARD_H
