#ifndef CARD_H
#define CARD_H

#include <vector>
#include <string>

enum class Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum class Rank {
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    ACE = 14
};

class Card {
private:
    Suit suit;
    Rank rank;

public:
    Card(Suit s, Rank r);
    Suit getSuit() const;
    Rank getRank() const;
    std::string toString() const;
    bool operator==(const Card& other) const;
    bool operator<(const Card& other) const;
    bool operator>(const Card& other) const;
};

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck();
    void shuffle();
    Card drawCard();
    void reset();
    bool isEmpty() const;
    size_t size() const;
};

#endif // CARD_H 