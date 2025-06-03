#pragma once

#include "InfoSet.h"
#include <unordered_map>

//going to start with only 2 players allowed
class Game {
public:
    int actionCount;
    int playerCount = 2;
    std::vector<int> cards;
    std::vector<std::vector<int>> playerCardStates;
    std::unordered_map<int, InfoSet> infoSets;
    std::vector<int> actions;

    Game(int actionCount, int cardCount) : actionCount(actionCount) {
        playerCardStates = std::vector<std::vector<int>>(cardCount * (cardCount - 1));
        actions = std::vector<int>(actionCount);

        cards = std::vector<int>(cardCount);
        for (int i = 0; i < cardCount; ++i) {
            cards[i] = i;
        }
    };

    virtual int getKey(int currentPlayer, int history, int state) = 0;
    virtual bool isTerminal(int history) = 0;
    virtual int getCurrentPlayer(int history) = 0;
    virtual double getUtility(int history, int currentPlayer, int state) = 0;
};