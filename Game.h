#pragma once

#include "InfoSet.h"
#include <unordered_map>

//going to start with only 2 players allowed
class Game {
public:
    int actionCount;
    std::unordered_map<int, InfoSet> infoSets;

    Game(int actionCount) : actionCount(actionCount) {};

    virtual int &getKey(int &currentPlayer, int &history) = 0;
    virtual bool &isTerminal(int &history) = 0;
    virtual int &getCurrentPlayer(int &history) = 0;
    virtual double &getUtility(int &history, int &currentPlayer) = 0;
};