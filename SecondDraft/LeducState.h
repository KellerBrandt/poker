#pragma once

#include "GameEnums.h"
#include "GameState.h"

class LeducState : public GameState {
public:
    //Game status
	int round;
    int roundRaiseCount;
    bool isTerminalNode;
    Player currentPlayer;

    //Card information
    std::vector<Card> playerCards;
    std::vector<Card> deck;
    int communityCard;

    //Betting and pot
    std::vector<double> playerAnte;
    std::vector<std::vector<Action>> actions;
    std::vector<Chance> chances;

    //Player status
    std::vector<bool> playerFolded;
    std::vector<bool> playerIsWinner;

    LeducState();

    Player getCurrentPlayer() const override;

    bool isTerminal() const override;

    bool isChance() const override;

    double getUtility() const override;

    std::vector<Action> legalActions() const override;

    std::unique_ptr<GameState> clone() const override;

    void applyAction(Action action) override;
	
    // For chance nodes: returns all possible chance actions and their probabilities.
    // e.g., { (dealCard1, 0.25), (dealCard2, 0.25), ... }
    std::vector<std::pair<Chance, double>> chanceOutcomes() const override;

    void applyChance(Chance chance) override;

    long getKey() const override;

	void print() const override;
};
/*
actions: fold, raise/bet, call/check, deal card
players: player0, player1, chance

Leduc:
Players assigned cards: 33
*/