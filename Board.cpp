//
// Created by fourteener on 10/31/19.
//

#include <vector>
#include <cstdlib>

#include "Board.h"

using std::vector;

struct Tile {
    int x = -1, y = -1;
    Tile(int i, int j) {
        x = i;
        y = j;
    }
};

// In charge of determining if anything needs to happen with the board
void Board::pollBoard() {
    if (buildingBoard) {
        int mapDelay = 0;
        if (clock.getElapsedTime().asMilliseconds() >= mapDelay) {
            iterate();
            iterationsDone++;

            if (iterType == 1 && iterationsDone == firstIters) {
                iterType = 2;
                iterationsDone = 0;
            }
            if (iterType == 2 && iterationsDone == secondIters) {
                buildingBoard = false;
                floodFill();
            }
            clock.restart();
        }
    }
    if (regenerate) {
        newBoard();
    }
}


// Replaces the current board with a blank board and initializes the board class
void Board::blankBoard(int windowSize, int tileSize) {
    int numTiles = windowSize / tileSize;

    vector< vector<bool> > newTiles;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
            row.push_back(true);
        }
        newTiles.push_back(row);
    }
    tiles = newTiles;

    // Specific for iterations
    iterationsDone = 0;
    buildingBoard = false;
    iterType = 1;
    regenerate = false;

    // Cave building settings
    firstIters = 6;
    secondIters = 12;
    startChance = 0.45;
    makeWall = 5;
    bigArea = 1;
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

// Starts a board generation
void Board::newBoard () {
    if (buildingBoard)
        return;

    fillBoard();

    clock.restart();
    iterationsDone = 0;
    iterType = 1;
    buildingBoard = true;
    regenerate = false;
}

// Fills the board randomly based on start chance
void Board::fillBoard() {
    int numTiles = tiles.size();

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

    tiles = newBoard;
    fillWalls();
}

// Performs a smoothing iteration on the board
// This is done using cellular automata
void Board::iterate() {
    int numTiles = tiles.size();

    vector< vector<bool> > newBoard;
    for (int i = 0; i < numTiles; i++) {
        vector<bool> row;
        for (int j = 0; j < numTiles; j++) {
           row.push_back(isWall(i,j)) ;
        }
        newBoard.push_back(row);
    };

    tiles = newBoard;
    fillWalls();
}

// Is the tile a wall?
// Handles the actual cellular automata
bool Board::isWall(int x, int y) {
    bool isWall = tiles.at(x).at(y);

    if (iterType == 1) {
        int oneStep = getNearbyWalls(x,y,1);
        int twoStep = getNearbyWalls(x,y,2);
        if (oneStep >= makeWall || twoStep <= bigArea)
            return true;
        return false;
    }
    else if (iterType == 2) {
        int oneStep = getNearbyWalls(x,y,1);
        if (oneStep >= makeWall)
            return true;
        return false;
    }
    return false;
}

// Count the number of surrounding tiles which are walls
int Board::getNearbyWalls(int x, int y, int steps) {
    // There are 9 possible tiles
    int wallCount = 0;
    int mapSize = tiles.size();
    mapSize = mapSize - 1;

    for (int i = -1 * steps; i <= steps; i++) {
        for (int j = -1 * steps; j <= steps; j++) {
            int xT = x + i;
            int yT = y + j;
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
    int mapSize = tiles.size();
    mapSize--;
    for (int i = 0; i < mapSize; i++) {
        tiles.at(0).at(i) = true;
        tiles.at(i).at(0) = true;
        tiles.at(mapSize).at(i) = true;
        tiles.at(i).at(mapSize) = true;
    }
}

// Flood fills the entire map to make sure there's only one big area
void Board::floodFill() {

    int numMadeAir = 0;
    int mapSize = tiles.size();
    int totalTiles = mapSize * mapSize;

    // Pick a random air tile
    vector<Tile> floodTiles;
    while (true) {
        int sX = rand() % mapSize, sY = rand() % mapSize;
        if (!tiles.at(sX).at(sY)) {
            floodTiles.emplace_back(sX, sY);
            break;
        }
    }

    // Calculate the flood fill vector
    vector< vector<bool> > floodFill (mapSize, vector<bool>(mapSize, true));
    while (!floodTiles.empty()) {
        Tile currTile = floodTiles.back();
        floodTiles.pop_back();
        int xC = currTile.x;
        int yC = currTile.y;;
        if (xC < 0 || yC < 0 || xC >= mapSize || yC >= mapSize) {
            continue;
        }
        bool tileVal = !tiles.at(xC).at(yC);
        if (tileVal) {
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
    double pctAir = static_cast<double>(numMadeAir) / totalTiles;
    if (pctAir <= airPercent) {
        regenerate = true;
    }

    // Store the new map into the tiles vector
    tiles = floodFill;
}