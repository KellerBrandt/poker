#pragma once

#include "GameEnums.h"
#include "GameState.h"

class KuhnState : public GameState {
public:
    //Game progress and status
    bool isTerminalNode;
    Player currentPlayer;
    std::vector<Chance> chances;

    //Card information
    std::vector<Card> playerCards;
    std::vector<Card> deck;

    //Betting and pot
    std::vector<double> playerAnte;
    std::vector<Action> actions;

    //Player status
    std::vector<bool> playerFolded;
    std::vector<bool> playerIsWinner;

    KuhnState();

    Player getCurrentPlayer() const override;

    bool isTerminal() const override;

    bool isChance() const override;

    double getUtility() const override;

    std::vector<Action> legalActions() const override;

    std::unique_ptr<GameState> clone() const override;

    void applyAction(Action action) override;

    // Returns all possible chance actions and their probabilities.
    std::vector<std::pair<Chance, double>> chanceOutcomes() const override;

    void applyChance(Chance chance) override;

    int getKey() const override;

	void print() const override;
};