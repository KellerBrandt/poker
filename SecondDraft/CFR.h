#pragma once

#include "InfoSet.h"
#include "GameState.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

class CFR {
public:
    std::unordered_map<int, InfoSet> infosets;

    void runVanillaCFR(std::unique_ptr<GameState>& root, int iterations);

    double vanillaCFR(std::unique_ptr<GameState>& state, double p0, double p1, double pChance);
};

/*
actions: fold, raise/bet, call/check, deal card
players: player0, player1, chance

Leduc:
Players assigned cards: 33
*/