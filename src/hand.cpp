#include "hand.h"
#include <algorithm>
#include <map>
#include <set>

// Hand implementation
Hand::Hand() : rank(HandRank::HIGH_CARD) {}

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

void Hand::clear() {
    cards.clear();
    rank = HandRank::HIGH_CARD;
    bestHand.clear();
}

HandRank Hand::evaluate() {
    if (cards.size() < 5) {
        rank = HandRank::HIGH_CARD;
        return rank;
    }
    
    rank = HandEvaluator::evaluateHand(cards);
    bestHand = HandEvaluator::findBestHand(cards);
    return rank;
}

std::string Hand::getRankString() const {
    switch (rank) {
        case HandRank::HIGH_CARD: return "High Card";
        case HandRank::PAIR: return "Pair";
        case HandRank::TWO_PAIR: return "Two Pair";
        case HandRank::THREE_OF_A_KIND: return "Three of a Kind";
        case HandRank::STRAIGHT: return "Straight";
        case HandRank::FLUSH: return "Flush";
        case HandRank::FULL_HOUSE: return "Full House";
        case HandRank::FOUR_OF_A_KIND: return "Four of a Kind";
        case HandRank::STRAIGHT_FLUSH: return "Straight Flush";
        case HandRank::ROYAL_FLUSH: return "Royal Flush";
        default: return "Unknown";
    }
}

std::vector<Card> Hand::getBestHand() const {
    return bestHand;
}

std::vector<Card> Hand::getCards() const {
    return cards;
}

bool Hand::operator<(const Hand& other) const {
    if (rank != other.rank) {
        return static_cast<int>(rank) < static_cast<int>(other.rank);
    }
    
    // Same rank, compare by high card
    auto sortedCards = cards;
    auto otherSortedCards = other.cards;
    std::sort(sortedCards.begin(), sortedCards.end(), std::greater<Card>());
    std::sort(otherSortedCards.begin(), otherSortedCards.end(), std::greater<Card>());
    
    for (size_t i = 0; i < std::min(sortedCards.size(), otherSortedCards.size()); i++) {
        if (sortedCards[i] < otherSortedCards[i]) return true;
        if (otherSortedCards[i] < sortedCards[i]) return false;
    }
    
    return false;
}

bool Hand::operator==(const Hand& other) const {
    if (rank != other.rank) return false;
    
    auto sortedCards = cards;
    auto otherSortedCards = other.cards;
    std::sort(sortedCards.begin(), sortedCards.end());
    std::sort(otherSortedCards.begin(), otherSortedCards.end());
    
    return sortedCards == otherSortedCards;
}

// HandEvaluator implementation
HandRank HandEvaluator::evaluateHand(const std::vector<Card>& cards) {
    if (cards.size() < 5) return HandRank::HIGH_CARD;
    
    if (isRoyalFlush(cards)) return HandRank::ROYAL_FLUSH;
    if (isStraightFlush(cards)) return HandRank::STRAIGHT_FLUSH;
    if (isFourOfAKind(cards)) return HandRank::FOUR_OF_A_KIND;
    if (isFullHouse(cards)) return HandRank::FULL_HOUSE;
    if (isFlush(cards)) return HandRank::FLUSH;
    if (isStraight(cards)) return HandRank::STRAIGHT;
    if (isThreeOfAKind(cards)) return HandRank::THREE_OF_A_KIND;
    if (isTwoPair(cards)) return HandRank::TWO_PAIR;
    if (isPair(cards)) return HandRank::PAIR;
    
    return HandRank::HIGH_CARD;
}

std::vector<Card> HandEvaluator::findBestHand(const std::vector<Card>& cards) {
    if (cards.size() <= 5) return cards;
    
    // For simplicity, return the first 5 cards sorted
    // In a real implementation, this would find the best 5-card combination
    std::vector<Card> result = cards;
    std::sort(result.begin(), result.end(), std::greater<Card>());
    return std::vector<Card>(result.begin(), result.begin() + 5);
}

bool HandEvaluator::isFlush(const std::vector<Card>& cards) {
    if (cards.size() < 5) return false;
    
    Suit firstSuit = cards[0].getSuit();
    int count = 0;
    for (const auto& card : cards) {
        if (card.getSuit() == firstSuit) count++;
    }
    return count >= 5;
}

bool HandEvaluator::isStraight(const std::vector<Card>& cards) {
    if (cards.size() < 5) return false;
    
    std::vector<int> ranks;
    for (const auto& card : cards) {
        ranks.push_back(static_cast<int>(card.getRank()));
    }
    std::sort(ranks.begin(), ranks.end());
    
    // Check for regular straight
    for (size_t i = 0; i <= ranks.size() - 5; i++) {
        bool isStraight = true;
        for (size_t j = 0; j < 4; j++) {
            if (ranks[i + j + 1] - ranks[i + j] != 1) {
                isStraight = false;
                break;
            }
        }
        if (isStraight) return true;
    }
    
    // Check for Ace-low straight (A,2,3,4,5)
    if (ranks.back() == 14) { // Ace
        std::vector<int> aceLow = {2, 3, 4, 5, 14};
        bool hasAceLow = true;
        for (int rank : aceLow) {
            if (std::find(ranks.begin(), ranks.end(), rank) == ranks.end()) {
                hasAceLow = false;
                break;
            }
        }
        if (hasAceLow) return true;
    }
    
    return false;
}

bool HandEvaluator::isStraightFlush(const std::vector<Card>& cards) {
    return isFlush(cards) && isStraight(cards);
}

bool HandEvaluator::isRoyalFlush(const std::vector<Card>& cards) {
    if (!isStraightFlush(cards)) return false;
    
    // Check if it contains Ace and 10
    bool hasAce = false, hasTen = false;
    for (const auto& card : cards) {
        if (card.getRank() == Rank::ACE) hasAce = true;
        if (card.getRank() == Rank::TEN) hasTen = true;
    }
    
    return hasAce && hasTen;
}

bool HandEvaluator::isFourOfAKind(const std::vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    for (const auto& pair : rankCount) {
        if (pair.second >= 4) return true;
    }
    return false;
}

bool HandEvaluator::isFullHouse(const std::vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    bool hasThree = false, hasTwo = false;
    for (const auto& pair : rankCount) {
        if (pair.second >= 3) hasThree = true;
        else if (pair.second >= 2) hasTwo = true;
    }
    
    return hasThree && hasTwo;
}

bool HandEvaluator::isThreeOfAKind(const std::vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    for (const auto& pair : rankCount) {
        if (pair.second >= 3) return true;
    }
    return false;
}

bool HandEvaluator::isTwoPair(const std::vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    int pairCount = 0;
    for (const auto& pair : rankCount) {
        if (pair.second >= 2) pairCount++;
    }
    
    return pairCount >= 2;
}

bool HandEvaluator::isPair(const std::vector<Card>& cards) {
    std::map<Rank, int> rankCount;
    for (const auto& card : cards) {
        rankCount[card.getRank()]++;
    }
    
    for (const auto& pair : rankCount) {
        if (pair.second >= 2) return true;
    }
    return false;
} 