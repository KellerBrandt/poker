#pragma once

#include "Game.h"

class KuhnPoker : public Game {
public:
    KuhnPoker();

    int getKey(int currentPlayer, int history, int state) override;
    bool isTerminal(int history) override;
    int getCurrentPlayer(int history) override;
    double getUtility(int history, int currentPlayer, int state) override;
    void print();
};