#pragma once

#include "GameEnums.h"
#include "GameState.h"

class KuhnState : public GameState {
public:
    //Game progress and status
    Player currentPlayer;

    //Card information
    std::vector<Card> playerCards;

    //Betting and pot
    int pot;
    std::vector<int> playerAnte;
    std::vector<Action> actions;

    //Player status
    std::vector<bool> playerFolded;
    std::vector<bool> playerIsWinner;

    KuhnState();

    Player getCurrentPlayer() const override;

    bool isTerminal() const override;

    bool isChance() const override;

    double getUtility(Player player) const override;

    std::vector<Action> legalActions() const override;

    std::unique_ptr<GameState> clone() const override;

    void applyAction(Action action) override;
    void applyChance(Chance chance) override;

    // Returns all possible chance actions and their probabilities.
    std::vector<std::pair<Chance, double>> chanceOutcomes() const override;

    Player getKey(Player player) const override;
};