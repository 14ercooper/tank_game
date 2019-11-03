//
// Created by fourteener on 10/31/19.
//

#include <vector>
#include <cstdlib>

#include "Board.h"

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
    if (buildingBoard) {
        int mapDelay = 0; // This can be used to debug the cellular automatically
        if (clock.getElapsedTime().asMilliseconds() >= mapDelay) {
            iterate();
            iterationsDone++;

            // First type of iteration makes the general shape but makes a lot of small islands
            if (iterType == 1 && iterationsDone == firstIters) {
                iterType = 2;
                iterationsDone = 0;
            }

            // This smooths some more and gets rid of a lot of the small islands
            if (iterType == 2 && iterationsDone == secondIters) {
                buildingBoard = false;
                floodFill();
            }
            clock.restart();
        }
    }

    // If something causes a map regeneration, start one here
    // Putting it here helps to reduce recursion and stack usage is a lot of regenerations happen in a row
    if (regenerate) {
        newBoard();
    }
}


// Replaces the current board with a blank board and initializes the board class with variables
void Board::blankBoard(int windowSize, int tileSize) {

    // Initialize the board vector
    const int numTiles = windowSize / tileSize;
    tiles = vector< vector<bool> > (numTiles, vector<bool>(numTiles, true));

    // Stores the tile size
    this->tileSize = tileSize;

    // Makes sure iteration variables have the correct values
    iterationsDone = 0;
    buildingBoard = false;
    iterType = 1;
    regenerate = false;

    // Cave building settings
    // This is used by the cellular automata
    firstIters = 6;
    secondIters = 12;
    startChance = 0.45;
    makeWall = 5;
    bigArea = 2;
    airPercent = 0.55;
}

// Get the current board
vector< vector<bool> > Board::getBoard() {
    return tiles;
}

// Is a board being generated?
bool Board::isGenerating() {
    return buildingBoard;
}

// Get how big a tile on the board is
int Board::getTileSize() {
    return tileSize;
}

// Starts a board generation
void Board::newBoard () {

    // Don't build a board if one's being built
    if (buildingBoard)
        return;

    // Fill the board randomly
    fillBoard();

    // Make sure all variables are set to generate a board
    clock.restart();
    iterationsDone = 0;
    iterType = 1;
    buildingBoard = true;
    regenerate = false;
}

// Checks collision at the given point
bool Board::isColliding(double x, double y) {
    // Chop off the decimal to get the tile to check in the vector
    int xC = x;
    int yC = y;

    // Make sure it's in the world
    const int mapSize = (tiles.size()) - 1;
    if (xC < 0 || yC < 0 || xC > mapSize || yC > mapSize) {
        return true;
    }

    // Return collision
    return tiles.at(xC).at(yC);
}

// Fills the board randomly based on start chance
void Board::fillBoard() {
    // Figure out how big the old board was
    const int numTiles = tiles.size();

    // Fill a new board based on the start chance
    vector< vector<bool> > newBoard;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
            if (std::rand() < startChance * RAND_MAX) {
                row.push_back(true);
            }
            else {
                row.push_back(false);
            }
        }
        newBoard.push_back(row);
    }

    // Save this to the board
    tiles = newBoard;

    // Fill in the walls
    fillWalls();
}

// Performs a smoothing iteration on the board
// This is done using cellular automata
void Board::iterate() {
    const int numTiles = tiles.size();

    // Loop through all the tiles on the board
    vector< vector<bool> > newBoard;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
            // This calls the actual cellular automata function
           row.push_back(isWall(i,j)) ;
        }
        newBoard.push_back(row);
    };

    // Generating in a new variable means that the step operates on the whole board at once based on the previous board
    tiles = newBoard;

    // Make sure the walls exists
    fillWalls();
}

// Is the tile a wall?
// Handles the actual cellular automata
// The rules are based on how many walls are in a close area to the tile in question
bool Board::isWall(int x, int y) {
    // First type of iteration, makes the general shape
    if (iterType == 1) {
        int oneStep = getNearbyWalls(x,y,1);
        int twoStep = getNearbyWalls(x,y,2);
        int threeStep = getNearbyWalls(x,y,3);
        if (twoStep > 18 || threeStep > 38)
            return true;
        if ((oneStep >= makeWall || twoStep <= bigArea))
            return true;
        return false;
    }

    // Smooths the level a bit and helps to get rid of small islands of wall
    else if (iterType == 2) {
        int oneStep = getNearbyWalls(x,y,1);
        if (oneStep >= makeWall)
            return true;
        return false;
    }

    // Wasn't defined, so just call it air
    return false;
}

// Count the number of surrounding tiles which are walls
int Board::getNearbyWalls(int x, int y, int steps) {
    // Get some variables, the -1 makes the loop easier
    int wallCount = 0;
    const int mapSize = (tiles.size()) - 1;

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
                wallCount += tiles.at(xT).at(yT);
            }
        }
    }

    return wallCount;
}

// Makes sure the entire map is surrounded by walls
void Board::fillWalls() {
    const int mapSize = (tiles.size()) - 1;
    // Basically get the entire border of the map to wall
    for (int i = 0; i < mapSize; i++) {
        tiles.at(0).at(i) = true;
        tiles.at(i).at(0) = true;
        tiles.at(mapSize).at(i) = true;
        tiles.at(i).at(mapSize) = true;
    }
}

// Flood fills the entire map to make sure there's only one big area
void Board::floodFill() {

    // Flood fill bariables
    int numMadeAir = 0;
    const int mapSize = tiles.size();
    const int totalTiles = mapSize * mapSize;

    // Pick a random air tile to start the fill from
    vector<Tile> floodTiles;
    while (true) {
        int sX = rand() % mapSize, sY = rand() % mapSize;
        if (!tiles.at(sX).at(sY)) {
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
        bool tileVal = !tiles.at(xC).at(yC);
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
    if (pctAir <= airPercent) {
        regenerate = true;
    }

    // Store the new map into the tiles vector
    tiles = floodFill;
}