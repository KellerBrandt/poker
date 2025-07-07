#include "card.h"
#include <algorithm>
#include <random>
#include <sstream>

// Card implementation
Card::Card(Suit s, Rank r) : suit(s), rank(r) {}

Suit Card::getSuit() const {
    return suit;
}

Rank Card::getRank() const {
    return rank;
}

std::string Card::toString() const {
    std::string rankStr;
    switch (rank) {
        case Rank::ACE: rankStr = "Ace"; break;
        case Rank::KING: rankStr = "King"; break;
        case Rank::QUEEN: rankStr = "Queen"; break;
        case Rank::JACK: rankStr = "Jack"; break;
        case Rank::TEN: rankStr = "10"; break;
        case Rank::NINE: rankStr = "9"; break;
        case Rank::EIGHT: rankStr = "8"; break;
        case Rank::SEVEN: rankStr = "7"; break;
        case Rank::SIX: rankStr = "6"; break;
        case Rank::FIVE: rankStr = "5"; break;
        case Rank::FOUR: rankStr = "4"; break;
        case Rank::THREE: rankStr = "3"; break;
        case Rank::TWO: rankStr = "2"; break;
    }
    
    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "Hearts"; break;
        case Suit::DIAMONDS: suitStr = "Diamonds"; break;
        case Suit::CLUBS: suitStr = "Clubs"; break;
        case Suit::SPADES: suitStr = "Spades"; break;
    }
    
    return rankStr + " of " + suitStr;
}

bool Card::operator==(const Card& other) const {
    return suit == other.suit && rank == other.rank;
}

bool Card::operator<(const Card& other) const {
    return rank < other.rank;
}

bool Card::operator>(const Card& other) const {
    return rank > other.rank;
}

// Deck implementation
Deck::Deck() {
    reset();
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card Deck::drawCard() {
    if (isEmpty()) {
        throw std::runtime_error("Cannot draw from empty deck");
    }
    
    Card card = cards.back();
    cards.pop_back();
    return card;
}

void Deck::reset() {
    cards.clear();
    
    // Create all 52 cards
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }
}

bool Deck::isEmpty() const {
    return cards.empty();
}

size_t Deck::size() const {
    return cards.size();
} 