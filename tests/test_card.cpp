#include <cassert>
#include <iostream>
#include <vector>
#include "../src/card.h"

void testCardCreation() {
    std::cout << "Testing Card Creation..." << std::endl;
    
    Card card1(Suit::HEARTS, Rank::ACE);
    assert(card1.getSuit() == Suit::HEARTS);
    assert(card1.getRank() == Rank::ACE);
    
    Card card2(Suit::SPADES, Rank::TWO);
    assert(card2.getSuit() == Suit::SPADES);
    assert(card2.getRank() == Rank::TWO);
    
    std::cout << "✓ Card creation tests passed" << std::endl;
}

void testCardToString() {
    std::cout << "Testing Card toString..." << std::endl;
    
    Card aceHearts(Suit::HEARTS, Rank::ACE);
    Card kingSpades(Suit::SPADES, Rank::KING);
    Card tenDiamonds(Suit::DIAMONDS, Rank::TEN);
    Card twoClubs(Suit::CLUBS, Rank::TWO);
    
    assert(aceHearts.toString() == "Ace of Hearts");
    assert(kingSpades.toString() == "King of Spades");
    assert(tenDiamonds.toString() == "10 of Diamonds");
    assert(twoClubs.toString() == "2 of Clubs");
    
    std::cout << "✓ Card toString tests passed" << std::endl;
}

void testCardComparison() {
    std::cout << "Testing Card Comparison..." << std::endl;
    
    Card aceHearts(Suit::HEARTS, Rank::ACE);
    Card kingHearts(Suit::HEARTS, Rank::KING);
    Card aceSpades(Suit::SPADES, Rank::ACE);
    Card twoHearts(Suit::HEARTS, Rank::TWO);
    
    // Same card
    assert(aceHearts == aceHearts);
    
    // Different cards
    assert(!(aceHearts == kingHearts));
    assert(!(aceHearts == aceSpades));
    
    // Less than comparisons (by rank)
    assert(twoHearts < aceHearts);
    assert(kingHearts < aceHearts);
    assert(twoHearts < kingHearts);
    
    std::cout << "✓ Card comparison tests passed" << std::endl;
}

void testDeckCreation() {
    std::cout << "Testing Deck Creation..." << std::endl;
    
    Deck deck;
    assert(deck.size() == 52);
    assert(!deck.isEmpty());
    
    std::cout << "✓ Deck creation tests passed" << std::endl;
}

void testDeckShuffle() {
    std::cout << "Testing Deck Shuffle..." << std::endl;
    
    Deck deck1;
    Deck deck2;
    
    // Get first few cards from unshuffled deck
    std::vector<Card> originalCards;
    for (int i = 0; i < 5; i++) {
        originalCards.push_back(deck1.drawCard());
    }
    
    // Reset and shuffle
    deck1.reset();
    deck1.shuffle();
    
    // Get first few cards from shuffled deck
    std::vector<Card> shuffledCards;
    for (int i = 0; i < 5; i++) {
        shuffledCards.push_back(deck1.drawCard());
    }
    
    // Check that at least some cards are in different positions
    bool different = false;
    for (size_t i = 0; i < originalCards.size(); i++) {
        if (!(originalCards[i] == shuffledCards[i])) {
            different = true;
            break;
        }
    }
    
    assert(different); // Shuffle should change order
    
    std::cout << "✓ Deck shuffle tests passed" << std::endl;
}

void testDeckDraw() {
    std::cout << "Testing Deck Draw..." << std::endl;
    
    Deck deck;
    int initialSize = deck.size();
    
    // Draw all cards
    std::vector<Card> drawnCards;
    while (!deck.isEmpty()) {
        drawnCards.push_back(deck.drawCard());
    }
    
    assert(drawnCards.size() == 52);
    assert(deck.isEmpty());
    assert(deck.size() == 0);
    
    // Check for duplicates
    for (size_t i = 0; i < drawnCards.size(); i++) {
        for (size_t j = i + 1; j < drawnCards.size(); j++) {
            assert(!(drawnCards[i] == drawnCards[j]));
        }
    }
    
    std::cout << "✓ Deck draw tests passed" << std::endl;
}

void testDeckReset() {
    std::cout << "Testing Deck Reset..." << std::endl;
    
    Deck deck;
    deck.drawCard();
    deck.drawCard();
    
    assert(deck.size() == 50);
    
    deck.reset();
    assert(deck.size() == 52);
    assert(!deck.isEmpty());
    
    std::cout << "✓ Deck reset tests passed" << std::endl;
}

int main() {
    std::cout << "Running Card and Deck Tests..." << std::endl;
    
    testCardCreation();
    testCardToString();
    testCardComparison();
    testDeckCreation();
    testDeckShuffle();
    testDeckDraw();
    testDeckReset();
    
    std::cout << "\n🎉 All Card and Deck tests passed!" << std::endl;
    return 0;
} 