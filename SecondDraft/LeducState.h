#pragma once

#include "GameEnums.h"
#include "GameState.h"

class LeducState : public GameState {
public:
    //Game progress and status
    int round;
    Player currentPlayer;

    //Card information
    Card publicCard;
    std::vector<Card> playerCards;
    std::vector<Card> deck; //remaining cards in deck

    //Betting and pot
    int pot;
    int callCount; //number of calls made this round
    int raiseCount; //number of raises made this round
    std::vector<int> playerAnte;
    std::vector<Action> round1Actions;
    std::vector<Action> round2Actions;

    //Player status
    int winnerCount;
    int remainingPlayers;
    std::vector<bool> playerFolded;
    std::vector<bool> playerIsWinner;

    LeducState();

    Player getCurrentPlayer() const override;

    bool isTerminal() const override;

    bool isChance() const override;

    double getUtility(Player player) const override;

    std::vector<Action> legalActions() const override;

    std::unique_ptr<GameState> clone() const override;

    void applyAction(Action action) override;
    void applyChance(Chance chance) override;

    // For chance nodes: returns all possible chance actions and their probabilities.
    // e.g., { (dealCard1, 0.25), (dealCard2, 0.25), ... }
    std::vector<std::pair<Chance, double>> chanceOutcomes() const override;

    Player getKey(Player player) const override;
};