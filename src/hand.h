#ifndef HAND_H
#define HAND_H

#include "card.h"
#include <vector>
#include <string>

enum class HandRank {
    HIGH_CARD,
    PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

class Hand {
private:
    std::vector<Card> cards;
    HandRank rank;
    std::vector<Card> bestHand;

public:
    Hand();
    void addCard(const Card& card);
    void clear();
    HandRank evaluate();
    std::string getRankString() const;
    std::vector<Card> getBestHand() const;
    std::vector<Card> getCards() const;
    bool operator<(const Hand& other) const;
    bool operator==(const Hand& other) const;
};

class HandEvaluator {
public:
    static HandRank evaluateHand(const std::vector<Card>& cards);
    static std::vector<Card> findBestHand(const std::vector<Card>& cards);
    static bool isFlush(const std::vector<Card>& cards);
    static bool isStraight(const std::vector<Card>& cards);
    static bool isStraightFlush(const std::vector<Card>& cards);
    static bool isRoyalFlush(const std::vector<Card>& cards);
    static bool isFourOfAKind(const std::vector<Card>& cards);
    static bool isFullHouse(const std::vector<Card>& cards);
    static bool isThreeOfAKind(const std::vector<Card>& cards);
    static bool isTwoPair(const std::vector<Card>& cards);
    static bool isPair(const std::vector<Card>& cards);
};

#endif // HAND_H 