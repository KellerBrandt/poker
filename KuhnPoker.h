#pragma once

#include "Game.h"

class KuhnPoker : public Game {
public:
    

    KuhnPoker(int actionCount) : Game(actionCount) {};
    ~KuhnPoker();

    int &getKey(int &currentPlayer, int &history) override;
    bool &isTerminal(int &history) override;
    int &getCurrentPlayer(int &history) override;
    double &getUtility(int &history, int &currentPlayer) override;
};