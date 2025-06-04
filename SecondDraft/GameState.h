#pragma once

#include <vector>
#include <string>
#include <memory>
#include "GameEnums.h"

class GameState {
public:
	virtual Player getCurrentPlayer() const = 0;

    virtual bool isTerminal() const = 0;

    virtual bool isChance() const = 0;

    virtual double getUtility() const = 0;

    virtual std::vector<Action> legalActions() const = 0;

    virtual std::unique_ptr<GameState> clone() const = 0;

    virtual void applyAction(Action action) = 0;
    
    virtual void applyChance(Chance chance) = 0;

    // For chance nodes: returns all possible chance actions and their probabilities.
    // e.g., { (dealCard1, 0.25), (dealCard2, 0.25), ... }
    virtual std::vector<std::pair<Chance, double>> chanceOutcomes() const = 0;

    virtual int getKey() const = 0;

	virtual void print() const = 0;
};