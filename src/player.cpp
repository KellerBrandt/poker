#include "player.h"

Player::Player(const std::string& playerName, int startingChips) 
    : name(playerName), chips(startingChips), isActive(true), isAllIn(false), currentBet(0) {}

std::string Player::getName() const {
    return name;
}

int Player::getChips() const {
    return chips;
}

Hand Player::getHand() const {
    return hand;
}

bool Player::getIsActive() const {
    return isActive;
}

bool Player::getIsAllIn() const {
    return isAllIn;
}

int Player::getCurrentBet() const {
    return currentBet;
}

void Player::setChips(int newChips) {
    chips = newChips;
}

void Player::setIsActive(bool active) {
    isActive = active;
}

void Player::setIsAllIn(bool allIn) {
    isAllIn = allIn;
}

void Player::setCurrentBet(int bet) {
    currentBet = bet;
}

void Player::addCard(const Card& card) {
    hand.addCard(card);
}

void Player::clearHand() {
    hand.clear();
}

void Player::addChips(int amount) {
    chips += amount;
}

void Player::removeChips(int amount) {
    chips -= amount;
}

void Player::resetBet() {
    currentBet = 0;
}

bool Player::canCheck(int currentBet) const {
    return currentBet == 0;
}

bool Player::canCall(int currentBet) const {
    return chips >= currentBet;
}

bool Player::canRaise(int currentBet, int minRaise) const {
    return chips >= (currentBet + minRaise);
}

bool Player::canAllIn() const {
    return chips >= 0;
} 