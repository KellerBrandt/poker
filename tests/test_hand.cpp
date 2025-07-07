#include <cassert>
#include <iostream>
#include <vector>
#include "../src/hand.h"

void testHandCreation() {
    std::cout << "Testing Hand Creation..." << std::endl;
    
    Hand hand;
    assert(hand.getCards().empty());
    assert(hand.getRankString() == "High Card");
    
    std::cout << "✓ Hand creation tests passed" << std::endl;
}

void testHandAddCard() {
    std::cout << "Testing Hand Add Card..." << std::endl;
    
    Hand hand;
    Card card1(Suit::HEARTS, Rank::ACE);
    Card card2(Suit::SPADES, Rank::KING);
    
    hand.addCard(card1);
    assert(hand.getCards().size() == 1);
    
    hand.addCard(card2);
    assert(hand.getCards().size() == 2);
    
    std::cout << "✓ Hand add card tests passed" << std::endl;
}

void testHandClear() {
    std::cout << "Testing Hand Clear..." << std::endl;
    
    Hand hand;
    hand.addCard(Card(Suit::HEARTS, Rank::ACE));
    hand.addCard(Card(Suit::SPADES, Rank::KING));
    
    assert(hand.getCards().size() == 2);
    
    hand.clear();
    assert(hand.getCards().empty());
    
    std::cout << "✓ Hand clear tests passed" << std::endl;
}

void testHighCard() {
    std::cout << "Testing High Card..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::TEN),
        Card(Suit::DIAMONDS, Rank::SEVEN),
        Card(Suit::CLUBS, Rank::FOUR),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::HIGH_CARD);
    
    std::cout << "✓ High card tests passed" << std::endl;
}

void testPair() {
    std::cout << "Testing Pair..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::ACE),
        Card(Suit::DIAMONDS, Rank::SEVEN),
        Card(Suit::CLUBS, Rank::FOUR),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::PAIR);
    
    std::cout << "✓ Pair tests passed" << std::endl;
}

void testTwoPair() {
    std::cout << "Testing Two Pair..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::ACE),
        Card(Suit::DIAMONDS, Rank::SEVEN),
        Card(Suit::CLUBS, Rank::SEVEN),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::TWO_PAIR);
    
    std::cout << "✓ Two pair tests passed" << std::endl;
}

void testThreeOfAKind() {
    std::cout << "Testing Three of a Kind..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::ACE),
        Card(Suit::DIAMONDS, Rank::ACE),
        Card(Suit::CLUBS, Rank::SEVEN),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::THREE_OF_A_KIND);
    
    std::cout << "✓ Three of a kind tests passed" << std::endl;
}

void testStraight() {
    std::cout << "Testing Straight..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::FIVE),
        Card(Suit::SPADES, Rank::SIX),
        Card(Suit::DIAMONDS, Rank::SEVEN),
        Card(Suit::CLUBS, Rank::EIGHT),
        Card(Suit::HEARTS, Rank::NINE)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::STRAIGHT);
    
    // Test Ace-low straight
    std::vector<Card> aceLowStraight = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::TWO),
        Card(Suit::DIAMONDS, Rank::THREE),
        Card(Suit::CLUBS, Rank::FOUR),
        Card(Suit::HEARTS, Rank::FIVE)
    };
    
    HandRank aceLowRank = HandEvaluator::evaluateHand(aceLowStraight);
    assert(aceLowRank == HandRank::STRAIGHT);
    
    std::cout << "✓ Straight tests passed" << std::endl;
}

void testFlush() {
    std::cout << "Testing Flush..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::HEARTS, Rank::TEN),
        Card(Suit::HEARTS, Rank::SEVEN),
        Card(Suit::HEARTS, Rank::FOUR),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::FLUSH);
    
    std::cout << "✓ Flush tests passed" << std::endl;
}

void testFullHouse() {
    std::cout << "Testing Full House..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::ACE),
        Card(Suit::DIAMONDS, Rank::ACE),
        Card(Suit::CLUBS, Rank::SEVEN),
        Card(Suit::HEARTS, Rank::SEVEN)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::FULL_HOUSE);
    
    std::cout << "✓ Full house tests passed" << std::endl;
}

void testFourOfAKind() {
    std::cout << "Testing Four of a Kind..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::ACE),
        Card(Suit::SPADES, Rank::ACE),
        Card(Suit::DIAMONDS, Rank::ACE),
        Card(Suit::CLUBS, Rank::ACE),
        Card(Suit::HEARTS, Rank::TWO)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::FOUR_OF_A_KIND);
    
    std::cout << "✓ Four of a kind tests passed" << std::endl;
}

void testStraightFlush() {
    std::cout << "Testing Straight Flush..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::FIVE),
        Card(Suit::HEARTS, Rank::SIX),
        Card(Suit::HEARTS, Rank::SEVEN),
        Card(Suit::HEARTS, Rank::EIGHT),
        Card(Suit::HEARTS, Rank::NINE)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::STRAIGHT_FLUSH);
    
    std::cout << "✓ Straight flush tests passed" << std::endl;
}

void testRoyalFlush() {
    std::cout << "Testing Royal Flush..." << std::endl;
    
    std::vector<Card> cards = {
        Card(Suit::HEARTS, Rank::TEN),
        Card(Suit::HEARTS, Rank::JACK),
        Card(Suit::HEARTS, Rank::QUEEN),
        Card(Suit::HEARTS, Rank::KING),
        Card(Suit::HEARTS, Rank::ACE)
    };
    
    HandRank rank = HandEvaluator::evaluateHand(cards);
    assert(rank == HandRank::ROYAL_FLUSH);
    
    std::cout << "✓ Royal flush tests passed" << std::endl;
}

void testHandComparison() {
    std::cout << "Testing Hand Comparison..." << std::endl;
    
    Hand hand1;
    hand1.addCard(Card(Suit::HEARTS, Rank::ACE));
    hand1.addCard(Card(Suit::SPADES, Rank::ACE));
    hand1.evaluate();
    
    Hand hand2;
    hand2.addCard(Card(Suit::HEARTS, Rank::KING));
    hand2.addCard(Card(Suit::SPADES, Rank::KING));
    hand2.evaluate();
    
    // Pair of aces should beat pair of kings
    assert(hand2 < hand1);
    assert(!(hand1 < hand2));
    
    std::cout << "✓ Hand comparison tests passed" << std::endl;
}

int main() {
    std::cout << "Running Hand Evaluation Tests..." << std::endl;
    
    testHandCreation();
    testHandAddCard();
    testHandClear();
    testHighCard();
    testPair();
    testTwoPair();
    testThreeOfAKind();
    testStraight();
    testFlush();
    testFullHouse();
    testFourOfAKind();
    testStraightFlush();
    testRoyalFlush();
    testHandComparison();
    
    std::cout << "\n🎉 All Hand Evaluation tests passed!" << std::endl;
    return 0;
} 