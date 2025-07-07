#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <string>

enum class PlayerAction {
    FOLD,
    CHECK,
    CALL,
    RAISE,
    ALL_IN
};

class Player {
private:
    std::string name;
    int chips;
    Hand hand;
    bool isActive;
    bool isAllIn;
    int currentBet;

public:
    Player(const std::string& playerName, int startingChips);
    
    // Getters
    std::string getName() const;
    int getChips() const;
    Hand getHand() const;
    bool getIsActive() const;
    bool getIsAllIn() const;
    int getCurrentBet() const;
    
    // Setters
    void setChips(int newChips);
    void setIsActive(bool active);
    void setIsAllIn(bool allIn);
    void setCurrentBet(int bet);
    
    // Game actions
    void addCard(const Card& card);
    void clearHand();
    void addChips(int amount);
    void removeChips(int amount);
    void resetBet();
    
    // Player actions
    bool canCheck(int currentBet) const;
    bool canCall(int currentBet) const;
    bool canRaise(int currentBet, int minRaise) const;
    bool canAllIn() const;
};

#endif // PLAYER_H 