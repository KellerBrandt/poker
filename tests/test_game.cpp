#include <cassert>
#include <iostream>
#include "../src/game.h"

void testGameCreation() {
    std::cout << "Testing Game Creation..." << std::endl;
    
    Game game(4, 1000, 10);
    
    assert(game.getCurrentState() == GameState::PREFLOP);
    assert(game.getCurrentRound() == BettingRound::PREFLOP);
    assert(game.getPot() == 0);
    assert(game.getCurrentBet() == 0);
    assert(game.getCommunityCards().empty());
    
    std::cout << "✓ Game creation tests passed" << std::endl;
}

void testGameAddPlayer() {
    std::cout << "Testing Game Add Player..." << std::endl;
    
    Game game(4, 1000, 10);
    
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    auto players = game.getPlayers();
    assert(players.size() == 4);
    assert(players[0]->getName() == "Alice");
    assert(players[1]->getName() == "Bob");
    assert(players[2]->getName() == "Charlie");
    assert(players[3]->getName() == "David");
    
    std::cout << "✓ Game add player tests passed" << std::endl;
}

void testGameStartNewHand() {
    std::cout << "Testing Game Start New Hand..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    game.startNewHand();
    
    assert(game.getCurrentState() == GameState::PREFLOP);
    assert(game.getCurrentRound() == BettingRound::PREFLOP);
    assert(game.getPot() == 0);
    assert(game.getCurrentBet() == 0);
    assert(game.getCommunityCards().empty());
    
    // Check that all players have empty hands
    auto players = game.getPlayers();
    for (auto& player : players) {
        assert(player->getHand().getCards().empty());
        assert(player->getCurrentBet() == 0);
        assert(player->getIsActive());
        assert(!player->getIsAllIn());
    }
    
    std::cout << "✓ Game start new hand tests passed" << std::endl;
}

void testGameDealCards() {
    std::cout << "Testing Game Deal Cards..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    game.startNewHand();
    game.dealCards();
    
    // Check that each player has 2 cards
    auto players = game.getPlayers();
    for (auto& player : players) {
        assert(player->getHand().getCards().size() == 2);
    }
    
    std::cout << "✓ Game deal cards tests passed" << std::endl;
}

void testGameNextRound() {
    std::cout << "Testing Game Next Round..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    game.startNewHand();
    
    // Test round progression
    assert(game.getCurrentRound() == BettingRound::PREFLOP);
    
    game.nextRound();
    assert(game.getCurrentRound() == BettingRound::FLOP);
    assert(game.getCurrentState() == GameState::FLOP);
    
    game.nextRound();
    assert(game.getCurrentRound() == BettingRound::TURN);
    assert(game.getCurrentState() == GameState::TURN);
    
    game.nextRound();
    assert(game.getCurrentRound() == BettingRound::RIVER);
    assert(game.getCurrentState() == GameState::RIVER);
    
    game.nextRound();
    assert(game.getCurrentState() == GameState::SHOWDOWN);
    
    std::cout << "✓ Game next round tests passed" << std::endl;
}

void testGameDealCommunityCards() {
    std::cout << "Testing Game Deal Community Cards..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    game.startNewHand();
    game.dealCards();
    
    // Deal flop (3 cards)
    game.dealCommunityCards(3);
    assert(game.getCommunityCards().size() == 3);
    
    // Deal turn (1 card)
    game.dealCommunityCards(1);
    assert(game.getCommunityCards().size() == 4);
    
    // Deal river (1 card)
    game.dealCommunityCards(1);
    assert(game.getCommunityCards().size() == 5);
    
    std::cout << "✓ Game deal community cards tests passed" << std::endl;
}

void testGameAddToPot() {
    std::cout << "Testing Game Add To Pot..." << std::endl;
    
    Game game(4, 1000, 10);
    
    assert(game.getPot() == 0);
    
    game.addToPot(100);
    assert(game.getPot() == 100);
    
    game.addToPot(50);
    assert(game.getPot() == 150);
    
    game.addToPot(0);
    assert(game.getPot() == 150);
    
    std::cout << "✓ Game add to pot tests passed" << std::endl;
}

void testGameIsHandComplete() {
    std::cout << "Testing Game Is Hand Complete..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    game.startNewHand();
    
    // Hand is not complete initially
    assert(!game.isHandComplete());
    
    // Progress through all rounds
    game.nextRound(); // FLOP
    assert(!game.isHandComplete());
    
    game.nextRound(); // TURN
    assert(!game.isHandComplete());
    
    game.nextRound(); // RIVER
    assert(!game.isHandComplete());
    
    game.nextRound(); // SHOWDOWN
    assert(game.isHandComplete());
    
    std::cout << "✓ Game is hand complete tests passed" << std::endl;
}

void testGameResetForNewHand() {
    std::cout << "Testing Game Reset For New Hand..." << std::endl;
    
    Game game(4, 1000, 10);
    game.addPlayer("Alice", 1000);
    game.addPlayer("Bob", 1000);
    game.addPlayer("Charlie", 1000);
    game.addPlayer("David", 1000);
    
    // Play through a hand
    game.startNewHand();
    game.dealCards();
    game.addToPot(100);
    game.dealCommunityCards(3);
    game.nextRound();
    game.dealCommunityCards(1);
    game.nextRound();
    game.dealCommunityCards(1);
    game.nextRound();
    game.nextRound();
    
    // Reset for new hand
    game.resetForNewHand();
    
    assert(game.getCurrentState() == GameState::PREFLOP);
    assert(game.getCurrentRound() == BettingRound::PREFLOP);
    assert(game.getPot() == 0);
    assert(game.getCurrentBet() == 0);
    assert(game.getCommunityCards().empty());
    
    // Check that all players have empty hands
    auto players = game.getPlayers();
    for (auto& player : players) {
        assert(player->getHand().getCards().empty());
        assert(player->getCurrentBet() == 0);
        assert(player->getIsActive());
        assert(!player->getIsAllIn());
    }
    
    std::cout << "✓ Game reset for new hand tests passed" << std::endl;
}

void testGameEdgeCases() {
    std::cout << "Testing Game Edge Cases..." << std::endl;
    
    // Game with 0 players
    Game game(0, 1000, 10);
    assert(game.getPlayers().empty());
    
    // Game with negative starting chips
    Game game2(2, -100, 10);
    game2.addPlayer("Alice", -100);
    game2.addPlayer("Bob", -100);
    
    // Game with 0 small blind
    Game game3(2, 1000, 0);
    game3.addPlayer("Alice", 1000);
    game3.addPlayer("Bob", 1000);
    
    std::cout << "✓ Game edge cases tests passed" << std::endl;
}

int main() {
    std::cout << "Running Game Tests..." << std::endl;
    
    testGameCreation();
    testGameAddPlayer();
    testGameStartNewHand();
    testGameDealCards();
    testGameNextRound();
    testGameDealCommunityCards();
    testGameAddToPot();
    testGameIsHandComplete();
    testGameResetForNewHand();
    testGameEdgeCases();
    
    std::cout << "\n🎉 All Game tests passed!" << std::endl;
    return 0;
} 