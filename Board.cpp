//
// Created by fourteener on 10/31/19.
//

#include <vector>
#include <cstdlib>

#include "Board.h"
#include "Game.h"

// I don't want to type vector all the time
using std::vector;

// Used to make the flood fill implementation easier and faster
struct Tile {
    int x = -1, y = -1;
    Tile(int i, int j) {
        x = i;
        y = j;
    }
};

// In charge of determining if anything needs to happen with the board
void Board::pollBoard() {

    // If the board is being generated, perform an iteration if mapDelay milliseconds have passed since the last tick
    if (_buildingBoard) {
        int mapDelay = 0; // This can be used to debug the cellular automatically
        if (_clock.getElapsedTime().asMilliseconds() >= mapDelay) {
            _iterate();
            _iterationsDone++;

            // First type of iteration makes the general shape but makes a lot of small islands
            if (_iterType == 1 && _iterationsDone == _firstIters) {
                _iterType = 2;
                _iterationsDone = 0;
            }

            // This smooths some more and gets rid of a lot of the small islands
            if (_iterType == 2 && _iterationsDone == _secondIters) {
                _buildingBoard = false;
                _floodFill();

                // Spawn enemies
                int points = Game::getGame()->getLevel() * 10;
                while (points > 0) {
                    _spawnEnemy(points);
                }
            }
            _clock.restart();
        }
    }

    // If something causes a map regeneration, start one here
    // Putting it here helps to reduce recursion and stack usage is a lot of regenerations happen in a row
    if (_regenerate) {
        newBoard();
    }
}


// Replaces the current board with a blank board and initializes the board class with variables
void Board::blankBoard(int windowSize, int tileSize) {

    // Initialize the board vector
    const int numTiles = windowSize / tileSize;
    _tiles = vector< vector<bool> > (numTiles, vector<bool>(numTiles, true));

    // Stores the tile size
    this->_tileSize = tileSize;

    // Makes sure iteration variables have the correct values
    _iterationsDone = 0;
    _buildingBoard = false;
    _iterType = 1;
    _regenerate = false;

    // Cave building settings
    // This is used by the cellular automata
    _firstIters = 6;
    _secondIters = 12;
    _startChance = 0.45;
    _makeWall = 5;
    _bigArea = 2;
    _airPercent = 0.55;
}

// Get the current board
vector< vector<bool> > Board::getBoard() {
    return _tiles;
}

// Is a board being generated?
bool Board::isGenerating() {
    return _buildingBoard;
}

// Get how big a tile on the board is
int Board::getTileSize() {
    return _tileSize;
}

// Starts a board generation
void Board::newBoard () {

    // Don't build a board if one's being built
    if (_buildingBoard)
        return;

    // Reset the weapons and enemies from the game
    Game::getGame()->resetAddedObjects();

    // Fill the board randomly
    _fillBoard();

    // Make sure all variables are set to generate a board
    _clock.restart();
    _iterationsDone = 0;
    _iterType = 1;
    _buildingBoard = true;
    _regenerate = false;
}

// Checks collision at the given point
bool Board::isColliding(double x, double y) {
    // Chop off the decimal to get the tile to check in the vector
    int xC = x;
    int yC = y;

    // Make sure it's in the world
    const int mapSize = (_tiles.size()) - 1;
    if (xC < 0 || yC < 0 || xC > mapSize || yC > mapSize) {
        return true;
    }

    // Return collision
    return _tiles.at(xC).at(yC);
}

// Fills the board randomly based on start chance
void Board::_fillBoard() {
    // Figure out how big the old board was
    const int numTiles = _tiles.size();

    // Fill a new board based on the start chance
    vector< vector<bool> > newBoard;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
            if (std::rand() < _startChance * RAND_MAX) {
                row.push_back(true);
            }
            else {
                row.push_back(false);
            }
        }
        newBoard.push_back(row);
    }

    // Save this to the board
    _tiles = newBoard;

    // Fill in the walls
    _fillWalls();
}

// Performs a smoothing iteration on the board
// This is done using cellular automata
void Board::_iterate() {
    const int numTiles = _tiles.size();

    // Loop through all the tiles on the board
    vector< vector<bool> > newBoard;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
            // This calls the actual cellular automata function
           row.push_back(_isWall(i, j)) ;
        }
        newBoard.push_back(row);
    };

    // Generating in a new variable means that the step operates on the whole board at once based on the previous board
    _tiles = newBoard;

    // Make sure the walls exists
    _fillWalls();
}

// Is the tile a wall?
// Handles the actual cellular automata
// The rules are based on how many walls are in a close area to the tile in question
bool Board::_isWall(int x, int y) {
    // First type of iteration, makes the general shape
    if (_iterType == 1) {
        int oneStep = _getNearbyWalls(x, y, 1);
        int twoStep = _getNearbyWalls(x, y, 2);
        int threeStep = _getNearbyWalls(x, y, 3);
        if (twoStep > 18 || threeStep > 38)
            return true;
        if ((oneStep >= _makeWall || twoStep <= _bigArea))
            return true;
        return false;
    }

    // Smooths the level a bit and helps to get rid of small islands of wall
    else if (_iterType == 2) {
        int oneStep = _getNearbyWalls(x, y, 1);
        if (oneStep >= _makeWall)
            return true;
        return false;
    }

    // Wasn't defined, so just call it air
    return false;
}

// Count the number of surrounding tiles which are walls
int Board::_getNearbyWalls(int x, int y, int steps) {
    // Get some variables, the -1 makes the loop easier
    int wallCount = 0;
    const int mapSize = (_tiles.size()) - 1;

    // Loop through all possible tiles within the range, counting how many are walls
    for (int i = -1 * steps; i <= steps; i++) {
        for (int j = -1 * steps; j <= steps; j++) {
            int xT = x + i;
            int yT = y + j;
            // Prevents out of bounds access errors, treats out of bounds as a wall
            if (xT < 0 || yT < 0 || xT > mapSize || yT > mapSize) {
                wallCount++;
            }
            else {
                wallCount += _tiles.at(xT).at(yT);
            }
        }
    }

    return wallCount;
}

// Makes sure the entire map is surrounded by walls
void Board::_fillWalls() {
    const int mapSize = (_tiles.size()) - 1;
    // Basically get the entire border of the map to wall
    for (int i = 0; i < mapSize; i++) {
        _tiles.at(0).at(i) = true;
        _tiles.at(i).at(0) = true;
        _tiles.at(mapSize).at(i) = true;
        _tiles.at(i).at(mapSize) = true;
    }
}

// Flood fills the entire map to make sure there's only one big area
void Board::_floodFill() {

    // Flood fill variables
    int numMadeAir = 0;
    const int mapSize = _tiles.size();
    const int totalTiles = mapSize * mapSize;

    // Pick a random air tile to start the fill from
    vector<Tile> floodTiles;
    while (true) {
        int sX = rand() % mapSize, sY = rand() % mapSize;
        if (!_tiles.at(sX).at(sY)) {
            floodTiles.emplace_back(sX, sY);
            break;
        }
    }

    // Calculate the flood fill vector (yes the vector and function have the same name)
    // This is done by starting at the air tile from above, then checking all adjacent tiles to see if they
    // Are also air, and if so, add them to the list to check, expanding through the entire open area
    vector< vector<bool> > floodFill (mapSize, vector<bool>(mapSize, true));
    while (!floodTiles.empty()) {
        // Grab the last tile from the list
        Tile currTile = floodTiles.back();
        floodTiles.pop_back();

        // Extract the location for easier access
        int xC = currTile.x;
        int yC = currTile.y;;

        // If it's outside the map, don't try to access it
        if (xC < 0 || yC < 0 || xC >= mapSize || yC >= mapSize) {
            continue;
        }

        //  If the tile is air, make note of that and add adjacent tiles to the check list
        bool tileVal = !_tiles.at(xC).at(yC);
        if (tileVal) {
            // Prevents two tiles from bouncing back and forth with each other
            if (!floodFill.at(xC).at(yC))
                continue;

            numMadeAir++;
            floodFill.at(xC).at(yC) = false;
            floodTiles.emplace_back(xC+1,yC);
            floodTiles.emplace_back(xC,yC+1);
            floodTiles.emplace_back(xC-1,yC);
            floodTiles.emplace_back(xC,yC-1);
        }
    }

    // Make sure at least airPercent of the map is air
    // Otherwise trigger a map regeneration
    double pctAir = static_cast<double>(numMadeAir) / totalTiles;
    if (pctAir <= _airPercent) {
        _regenerate = true;
    }

    // Store the new map into the tiles vector
    _tiles = floodFill;
}

// Handles the spawning of enemies
void Board::_spawnEnemy(int &points) {
    // Where to spawn the enemy?
    int xPos = -1, yPos = 1;
    while (true) {
        xPos = rand() % _tiles.at(0).size();
        yPos = rand() % _tiles.size();
        if (!isColliding(xPos, yPos)) {
            break;
        }
    }

    // Spawn an enemy
    while (true) {
        int type = rand() % 5;
        switch (type) {
            // Spawn a stupid stationary enemy
            case 0: {
                if (points < 1)
                    continue;
                Enemy e(2, 200, 1, sf::Color::Yellow, xPos, yPos, 0, false);
                Game::getGame()->addEnemy(e);
                points -= 1;
                return;
            }
            // Spawn smart stationary enemy
            case 1: {
                if (points < 3)
                    continue;
                Enemy e(2.5, 200, 1, sf::Color::Magenta, xPos, yPos, 0, true);
                Game::getGame()->addEnemy(e);
                points -= 3;
                return;
            }
            // Green tank, the player will probably die
            case 2: {
                if (points < 15)
                    continue;
                Enemy e(1.5, 800, 4, sf::Color::Cyan, xPos, yPos, 0, true);
                Game::getGame()->addEnemy(e);
                points -= 15;
                return;
            }
            // Mobile, slow, smart
            case 3: {
                if (points < 5)
                    continue;
                Enemy e(5, 200, 2, sf::Color::White, xPos, yPos, 1.5 * Game::getGame()->getTileSize(), true);
                Game::getGame()->addEnemy(e);
                points -= 5;
                return;
            }
            // Mobile, faster and smart
            case 4: {
                if (points < 9)
                    continue;
                Enemy e(3, 350, 3, sf::Color::Black, xPos, yPos, 3.5 * Game::getGame()->getTileSize(), true);
                Game::getGame()->addEnemy(e);
                points -= 9;
                return;
            }
            // Skip spawning an enemy, but also doesn't change anything
            default:
                return;
        }
    }
}