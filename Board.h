//
// Created by fourteener on 10/31/19.
//

#ifndef SFML_PRACTICE_BOARD_H
#define SFML_PRACTICE_BOARD_H

#include <vector>
#include <SFML/System.hpp>

class Board {
public:
    void pollBoard();
    void blankBoard(int windowSize, int tileSize);
    void newBoard();
    bool isGenerating();
    std::vector< std::vector<bool> > getBoard();


private:
    std::vector< std::vector<bool> > tiles;
    int iterationsDone;
    sf::Clock clock;
    bool buildingBoard;
    int iterType;
    bool regenerate;
    void fillBoard();
    void iterate();
    bool isWall(int x, int y);
    int getNearbyWalls(int x, int y, int steps);
    void fillWalls();
    void floodFill();

    // Cave generation constants
    int firstIters;
    int secondIters;
    double startChance;
    int makeWall;
    int bigArea;
    double airPercent;
};

#endif //SFML_PRACTICE_BOARD_H
