#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "player.h"
#include "hand.h"
#include <vector>
#include <memory>

enum class GameState {
    PREFLOP,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN,
    GAME_OVER
};

enum class BettingRound {
    PREFLOP,
    FLOP,
    TURN,
    RIVER
};

class Game {
private:
    std::vector<std::shared_ptr<Player>> players;
    Deck deck;
    std::vector<Card> communityCards;
    GameState currentState;
    BettingRound currentRound;
    int pot;
    int currentBet;
    int dealerIndex;
    int currentPlayerIndex;
    int smallBlind;
    int bigBlind;

public:
    Game(int numPlayers, int startingChips, int smallBlindAmount);
    
    // Game setup
    void addPlayer(const std::string& name, int chips);
    void startNewHand();
    void dealCards();
    
    // Game flow
    void nextRound();
    void playBettingRound();
    void showdown();
    void awardPot();
    
    // Getters
    GameState getCurrentState() const;
    BettingRound getCurrentRound() const;
    int getPot() const;
    int getCurrentBet() const;
    std::vector<Card> getCommunityCards() const;
    std::vector<std::shared_ptr<Player>> getPlayers() const;
    
    // Game actions
    void addToPot(int amount);
    void dealCommunityCards(int count);
    void moveToNextPlayer();
    bool isHandComplete() const;
    void resetForNewHand();
};

#endif // GAME_H 