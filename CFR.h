#pragma once

#include "Game.h"
#include <vector>

class CFR {
public:
    void runVanillaCFR(Game &game);
    double vanillaCFR(Game &game, int history, double p1, double p2, double &pChance);
};