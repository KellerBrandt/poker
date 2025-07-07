#include <cassert>
#include <iostream>
#include "../src/player.h"

void testPlayerCreation() {
    std::cout << "Testing Player Creation..." << std::endl;
    
    Player player("Alice", 1000);
    
    assert(player.getName() == "Alice");
    assert(player.getChips() == 1000);
    assert(player.getIsActive());
    assert(!player.getIsAllIn());
    assert(player.getCurrentBet() == 0);
    
    std::cout << "✓ Player creation tests passed" << std::endl;
}

void testPlayerChips() {
    std::cout << "Testing Player Chips..." << std::endl;
    
    Player player("Bob", 1000);
    
    // Add chips
    player.addChips(500);
    assert(player.getChips() == 1500);
    
    // Remove chips
    player.removeChips(300);
    assert(player.getChips() == 1200);
    
    // Set chips directly
    player.setChips(2000);
    assert(player.getChips() == 2000);
    
    std::cout << "✓ Player chips tests passed" << std::endl;
}

void testPlayerHand() {
    std::cout << "Testing Player Hand..." << std::endl;
    
    Player player("Charlie", 1000);
    
    // Initially empty hand
    assert(player.getHand().getCards().empty());
    
    // Add cards
    Card card1(Suit::HEARTS, Rank::ACE);
    Card card2(Suit::SPADES, Rank::KING);
    
    player.addCard(card1);
    player.addCard(card2);
    
    assert(player.getHand().getCards().size() == 2);
    
    // Clear hand
    player.clearHand();
    assert(player.getHand().getCards().empty());
    
    std::cout << "✓ Player hand tests passed" << std::endl;
}

void testPlayerBetting() {
    std::cout << "Testing Player Betting..." << std::endl;
    
    Player player("David", 1000);
    
    // Initial state
    assert(player.getCurrentBet() == 0);
    
    // Set bet
    player.setCurrentBet(100);
    assert(player.getCurrentBet() == 100);
    
    // Reset bet
    player.resetBet();
    assert(player.getCurrentBet() == 0);
    
    std::cout << "✓ Player betting tests passed" << std::endl;
}

void testPlayerActions() {
    std::cout << "Testing Player Actions..." << std::endl;
    
    Player player("Eve", 1000);
    
    // Can check when no current bet
    assert(player.canCheck(0));
    assert(!player.canCheck(100));
    
    // Can call when has enough chips
    assert(player.canCall(100));
    assert(player.canCall(1000));
    assert(!player.canCall(1500));
    
    // Can raise when has enough chips and min raise is reasonable
    assert(player.canRaise(100, 50));
    assert(!player.canRaise(100, 1000));
    
    // Can go all in
    assert(player.canAllIn());
    
    // Test with fewer chips
    player.setChips(50);
    assert(!player.canCall(100));
    assert(!player.canRaise(100, 50));
    assert(player.canAllIn());
    
    std::cout << "✓ Player actions tests passed" << std::endl;
}

void testPlayerAllIn() {
    std::cout << "Testing Player All In..." << std::endl;
    
    Player player("Frank", 1000);
    
    assert(!player.getIsAllIn());
    
    player.setIsAllIn(true);
    assert(player.getIsAllIn());
    
    player.setIsAllIn(false);
    assert(!player.getIsAllIn());
    
    std::cout << "✓ Player all in tests passed" << std::endl;
}

void testPlayerActive() {
    std::cout << "Testing Player Active State..." << std::endl;
    
    Player player("Grace", 1000);
    
    assert(player.getIsActive());
    
    player.setIsActive(false);
    assert(!player.getIsActive());
    
    player.setIsActive(true);
    assert(player.getIsActive());
    
    std::cout << "✓ Player active state tests passed" << std::endl;
}

void testPlayerEdgeCases() {
    std::cout << "Testing Player Edge Cases..." << std::endl;
    
    // Player with 0 chips
    Player player("Henry", 0);
    assert(player.getChips() == 0);
    assert(!player.canCall(1));
    assert(!player.canRaise(0, 1));
    assert(player.canAllIn());
    
    // Player with negative chips (shouldn't happen but test anyway)
    player.setChips(-100);
    assert(player.getChips() == -100);
    
    // Remove more chips than available
    player.setChips(100);
    player.removeChips(200);
    assert(player.getChips() == -100);
    
    std::cout << "✓ Player edge cases tests passed" << std::endl;
}

int main() {
    std::cout << "Running Player Tests..." << std::endl;
    
    testPlayerCreation();
    testPlayerChips();
    testPlayerHand();
    testPlayerBetting();
    testPlayerActions();
    testPlayerAllIn();
    testPlayerActive();
    testPlayerEdgeCases();
    
    std::cout << "\n🎉 All Player tests passed!" << std::endl;
    return 0;
} 