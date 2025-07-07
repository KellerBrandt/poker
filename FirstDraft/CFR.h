#pragma once

#include "Game.h"
#include <vector>

class CFR {
public:
    void runVanillaCFR(Game &game, int iterations);
    double vanillaCFR(Game &game, int &state, int history, double p1, double p2, double pChance);
};