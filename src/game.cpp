#include "game.h"

Game::Game(int numPlayers, int startingChips, int smallBlindAmount) 
    : currentState(GameState::PREFLOP), currentRound(BettingRound::PREFLOP), 
      pot(0), currentBet(0), dealerIndex(0), currentPlayerIndex(0), 
      smallBlind(smallBlindAmount), bigBlind(smallBlindAmount * 2) {}

void Game::addPlayer(const std::string& name, int chips) {
    players.push_back(std::make_shared<Player>(name, chips));
}

void Game::startNewHand() {
    // Reset game state
    currentState = GameState::PREFLOP;
    currentRound = BettingRound::PREFLOP;
    pot = 0;
    currentBet = 0;
    currentPlayerIndex = 0;
    
    // Clear community cards
    communityCards.clear();
    
    // Reset deck
    deck.reset();
    deck.shuffle();
    
    // Clear all player hands and reset bets
    for (auto& player : players) {
        player->clearHand();
        player->resetBet();
        player->setIsActive(true);
        player->setIsAllIn(false);
    }
}

void Game::dealCards() {
    // Deal 2 cards to each player
    for (int round = 0; round < 2; round++) {
        for (auto& player : players) {
            if (player->getIsActive()) {
                player->addCard(deck.drawCard());
            }
        }
    }
}

void Game::nextRound() {
    switch (currentRound) {
        case BettingRound::PREFLOP:
            currentRound = BettingRound::FLOP;
            currentState = GameState::FLOP;
            break;
        case BettingRound::FLOP:
            currentRound = BettingRound::TURN;
            currentState = GameState::TURN;
            break;
        case BettingRound::TURN:
            currentRound = BettingRound::RIVER;
            currentState = GameState::RIVER;
            break;
        case BettingRound::RIVER:
            currentState = GameState::SHOWDOWN;
            break;
    }
    
    // Reset current bet for new round
    currentBet = 0;
    for (auto& player : players) {
        player->resetBet();
    }
}

void Game::playBettingRound() {
    // This is a placeholder - actual betting logic would be implemented here
    // For now, just move to the next round
    nextRound();
}

void Game::showdown() {
    currentState = GameState::SHOWDOWN;
    // Hand evaluation and winner determination would go here
}

void Game::awardPot() {
    // Pot distribution logic would go here
    pot = 0;
}

GameState Game::getCurrentState() const {
    return currentState;
}

BettingRound Game::getCurrentRound() const {
    return currentRound;
}

int Game::getPot() const {
    return pot;
}

int Game::getCurrentBet() const {
    return currentBet;
}

std::vector<Card> Game::getCommunityCards() const {
    return communityCards;
}

std::vector<std::shared_ptr<Player>> Game::getPlayers() const {
    return players;
}

void Game::addToPot(int amount) {
    if (amount > 0) {
        pot += amount;
    }
}

void Game::dealCommunityCards(int count) {
    for (int i = 0; i < count; i++) {
        if (!deck.isEmpty()) {
            communityCards.push_back(deck.drawCard());
        }
    }
}

void Game::moveToNextPlayer() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

bool Game::isHandComplete() const {
    return currentState == GameState::SHOWDOWN || currentState == GameState::GAME_OVER;
}

void Game::resetForNewHand() {
    startNewHand();
} 