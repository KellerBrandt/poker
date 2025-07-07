#include "LeducState.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <cmath>

// Leduc Hold'em Rules (corrected):
// - Deck: 6 cards (2 each of J, Q, K)
// - Round 0: Each player gets 1 private card, betting with 1 chip
// - Round 1: 1 community card, betting with 2 chips
// - Betting: Fixed-limit (1 chip round 0, 2 chips round 1)
// - Max raises per round: 2-3 (implementation uses 2)
// - Winning: Pair wins, else highest card wins

void test_constructor() {
    LeducState state;
    assert(state.round == 0);
    assert(state.roundRaiseCount == 0);
    assert(state.isTerminalNode == false);
    assert(state.currentPlayer == Player::CHANCE);
    assert(state.deck.size() == 6);
    assert(state.playerAnte[0] == 1.0 && state.playerAnte[1] == 1.0);
    assert(!state.playerFolded[0] && !state.playerFolded[1]);
    assert(!state.playerIsWinner[0] && !state.playerIsWinner[1]);
    assert(state.actions.size() == 2);
    assert(state.actions[0].empty() && state.actions[1].empty());
    std::cout << "Constructor test passed.\n";
}

void test_clone() {
    LeducState state;
    state.round = 1;
    state.roundRaiseCount = 1;
    state.isTerminalNode = true;
    state.currentPlayer = Player::PLAYER1;
    state.playerCards = {Card::JACK, Card::QUEEN};
    state.deck = {Card::KING};
    state.communityCard = Card::KING;
    state.playerAnte = {2.0, 3.0};
    state.actions = {{Action::FOLD}, {Action::CALL}};
    state.chances = {Chance::DEAL_JACK, Chance::DEAL_QUEEN};
    state.playerFolded = {true, false};
    state.playerIsWinner = {false, true};
    
    auto clone = state.clone();
    LeducState* c = static_cast<LeducState*>(clone.get());
    
    assert(c->round == 1);
    assert(c->roundRaiseCount == 1);
    assert(c->isTerminalNode == true);
    assert(c->currentPlayer == Player::PLAYER1);
    assert(c->playerCards[0] == Card::JACK && c->playerCards[1] == Card::QUEEN);
    assert(c->deck[0] == Card::KING);
    assert(c->communityCard == Card::KING);
    assert(c->playerAnte[0] == 2.0 && c->playerAnte[1] == 3.0);
    assert(c->actions[0][0] == Action::FOLD && c->actions[1][0] == Action::CALL);
    assert(c->chances[0] == Chance::DEAL_JACK && c->chances[1] == Chance::DEAL_QUEEN);
    assert(c->playerFolded[0] && !c->playerFolded[1]);
    assert(!c->playerIsWinner[0] && c->playerIsWinner[1]);
    
    // Test that clone is independent
    state.round = 5;
    assert(c->round == 1); // Clone should not be affected
    std::cout << "Clone test passed.\n";
}

void test_legalActions() {
    LeducState state;
    state.currentPlayer = Player::PLAYER0;
    
    // Test normal case - can raise
    state.roundRaiseCount = 0;
    auto acts = state.legalActions();
    assert(acts.size() == 3);
    assert(acts[0] == Action::FOLD);
    assert(acts[1] == Action::CALL);
    assert(acts[2] == Action::RAISE);

    // Test when raise limit reached (2 raises max)
    state.roundRaiseCount = 2;
    acts = state.legalActions();
    assert(acts.size() == 2);
    assert(acts[0] == Action::FOLD);
    assert(acts[1] == Action::CALL);
    
    // Test edge case - exactly at raise limit
    state.roundRaiseCount = 1;
    acts = state.legalActions();
    assert(acts.size() == 3); // Should still be able to raise
    
    // Test terminal state
    state.isTerminalNode = true;
    acts = state.legalActions();
    assert(acts.empty());
    
    std::cout << "LegalActions test passed\n";
}

void test_applyAction_fold() {
    LeducState state;
    state.currentPlayer = Player::PLAYER0;
    state.playerAnte = {1.0, 1.0};
    state.actions = {{}, {}};
    
    state.applyAction(Action::FOLD);
    
    assert(state.playerFolded[Player::PLAYER0]);
    assert(state.isTerminalNode);
    assert(state.getCurrentPlayer() == Player::PLAYER1);
    assert(state.actions[0].size() == 1);
    assert(state.actions[0][0] == Action::FOLD);
    
    std::cout << "ApplyAction fold test passed\n";
}

void test_applyAction_call_round0() {
    LeducState state;
    state.currentPlayer = Player::PLAYER0;
    state.playerAnte = {1.0, 1.0};
    state.actions = {{}, {}};
    
    // Test first call (check)
    state.applyAction(Action::CALL);
    assert(state.getCurrentPlayer() == Player::PLAYER1);
    assert(state.round == 0);
    assert(state.actions[0].size() == 1);
    assert(state.actions[0][0] == Action::CALL);
    
    // Test second call (round ends)
    state.applyAction(Action::CALL);
    assert(state.getCurrentPlayer() == Player::CHANCE);
    assert(state.round == 1);
    assert(state.roundRaiseCount == 0);
    assert(state.actions[0].size() == 2);
    assert(state.actions[0][1] == Action::CALL);
    
    std::cout << "ApplyAction call round 0 test passed\n";
}

void test_applyAction_call_round1() {
    LeducState state;
    state.round = 1;
    state.currentPlayer = Player::PLAYER0;
    state.playerAnte = {2.0, 2.0}; // Round 1 starts with 2 chips each
    state.actions = {{Action::CALL, Action::CALL}, {}};
    
    // Test call in round 1
    state.applyAction(Action::CALL);
    assert(state.getCurrentPlayer() == Player::PLAYER1);
    assert(state.round == 1);
    
    // Test second call (game ends)
    state.applyAction(Action::CALL);
    assert(state.isTerminalNode);
    assert(state.round == 2);
    
    std::cout << "ApplyAction call round 1 test passed\n";
}

void test_applyAction_raise_round0() {
    LeducState state;
    state.currentPlayer = Player::PLAYER0;
    state.playerAnte = {1.0, 1.0};
    state.actions = {{}, {}};
    
    // Test first raise (should be 2 chips in round 0)
    state.applyAction(Action::RAISE);
    assert(state.getCurrentPlayer() == Player::PLAYER1);
    assert(state.round == 0);
    assert(state.roundRaiseCount == 1);
    assert(state.playerAnte[Player::PLAYER0] == 3.0); // 1 + 2
    assert(state.actions[0].size() == 1);
    assert(state.actions[0][0] == Action::RAISE);
    
    // Test call after raise
    state.applyAction(Action::CALL);
    assert(state.getCurrentPlayer() == Player::CHANCE);
    assert(state.round == 1);
    assert(state.roundRaiseCount == 0);
    assert(state.playerAnte[Player::PLAYER1] == 3.0); // Match the raise
    
    std::cout << "ApplyAction raise round 0 test passed\n";
}

void test_applyAction_raise_round1() {
    LeducState state;
    state.round = 1;
    state.currentPlayer = Player::PLAYER0;
    state.playerAnte = {2.0, 2.0}; // Round 1 starts with 2 chips each
    state.actions = {{Action::CALL, Action::CALL}, {}};
    
    // Test raise in round 1 (should be 4 chips in round 1)
    state.applyAction(Action::RAISE);
    assert(state.getCurrentPlayer() == Player::PLAYER1);
    assert(state.round == 1);
    assert(state.roundRaiseCount == 1);
    assert(state.playerAnte[Player::PLAYER0] == 6.0); // 2 + 4
    
    // Test call after raise
    state.applyAction(Action::CALL);
    assert(state.getCurrentPlayer() == Player::PLAYER0);
    assert(state.isTerminalNode);
    assert(state.playerAnte[Player::PLAYER1] == 6.0); // Match the raise
    
    std::cout << "ApplyAction raise round 1 test passed\n";
}

void test_applyAction_raise_limit() {
    LeducState state;
    state.currentPlayer = Player::PLAYER0;
    state.roundRaiseCount = 1;
    state.actions = {{Action::RAISE}, {}};
    
    // Test second raise
    state.applyAction(Action::RAISE);
    assert(state.roundRaiseCount == 2);
    
    // Test that no more raises are allowed
    auto acts = state.legalActions();
    assert(acts.size() == 2);
    assert(acts[0] == Action::FOLD);
    assert(acts[1] == Action::CALL);
    
    std::cout << "ApplyAction raise limit test passed\n";
}

void test_chanceOutcomes() {
    LeducState state;
    
    // Test initial deck (6 cards: 2 each of J, Q, K)
    auto outcomes = state.chanceOutcomes();
    assert(outcomes.size() == 3);
    
    // Check probabilities sum to 1.0
    double sum = 0.0;
    for (const auto& outcome : outcomes) {
        sum += outcome.second;
    }
    assert(std::abs(sum - 1.0) < 1e-6);
    
    // Check initial probabilities (2/6 = 1/3 for each card)
    for (const auto& outcome : outcomes) {
        assert(std::abs(outcome.second - 1.0/3.0) < 1e-6);
    }
    
    // Test after dealing one card
    state.applyChance(Chance::DEAL_JACK);
    outcomes = state.chanceOutcomes();
    assert(outcomes.size() == 3);
    
    // Check probabilities after dealing one J (1 J, 2 Q, 2 K left)
    sum = 0.0;
    for (const auto& outcome : outcomes) {
        sum += outcome.second;
    }
    assert(std::abs(sum - 1.0) < 1e-6);
    
    // Test empty deck
    state.deck.clear();
    outcomes = state.chanceOutcomes();
    assert(outcomes.empty());
    
    std::cout << "ChanceOutcomes test passed\n";
}

void test_applyChance_round0() {
    LeducState state;
    
    // Test first card dealt
    state.applyChance(Chance::DEAL_JACK);
    assert(state.playerCards.size() == 1);
    assert(state.playerCards[0] == Card::JACK);
    assert(state.chances.size() == 1);
    assert(state.chances[0] == Chance::DEAL_JACK);
    assert(state.deck.size() == 5);
    assert(state.getCurrentPlayer() == Player::CHANCE);
    
    // Test second card dealt
    state.applyChance(Chance::DEAL_QUEEN);
    assert(state.playerCards.size() == 2);
    assert(state.playerCards[1] == Card::QUEEN);
    assert(state.chances.size() == 2);
    assert(state.getCurrentPlayer() == Player::PLAYER0);
    
    std::cout << "ApplyChance round 0 test passed\n";
}

void test_applyChance_round1_winning_logic() {
    // Test winning logic - player 0 has pair
    LeducState state1;
    state1.round = 1;
    state1.playerCards = {Card::JACK, Card::QUEEN};
    state1.applyChance(Chance::DEAL_JACK);
    assert(state1.communityCard == Card::JACK);
    assert(state1.getCurrentPlayer() == Player::PLAYER0);
    assert(state1.playerIsWinner[0]);
    assert(!state1.playerIsWinner[1]);
    
    // Test winning logic - player 1 has pair
    LeducState state2;
    state2.round = 1;
    state2.playerCards = {Card::JACK, Card::QUEEN};
    state2.applyChance(Chance::DEAL_QUEEN);
    assert(state2.communityCard == Card::QUEEN);
    assert(state2.getCurrentPlayer() == Player::PLAYER0);
    assert(!state2.playerIsWinner[0]);
    assert(state2.playerIsWinner[1]);
    
    // Test winning logic - no pairs, higher card wins
    LeducState state3;
    state3.round = 1;
    state3.playerCards = {Card::JACK, Card::QUEEN};
    state3.applyChance(Chance::DEAL_KING);
    assert(state3.communityCard == Card::KING);
    assert(state3.getCurrentPlayer() == Player::PLAYER0);
    assert(!state3.playerIsWinner[0]);
    assert(state3.playerIsWinner[1]); // QUEEN > JACK
    
    // Test winning logic - tie (should not happen with current rules)
    LeducState state4;
    state4.round = 1;
    state4.playerCards = {Card::JACK, Card::JACK};
    state4.applyChance(Chance::DEAL_KING);
    assert(state4.communityCard == Card::KING);
    assert(state4.getCurrentPlayer() == Player::PLAYER0);
    // Both should be false in case of tie (split pot)
    assert(!state4.playerIsWinner[0]);
    assert(!state4.playerIsWinner[1]);
    
    std::cout << "ApplyChance round 1 winning logic test passed\n";
}

void test_getUtility() {
    LeducState state;
    state.playerAnte = {2.0, 3.0};
    
    // Test when current player folded
    state.currentPlayer = Player::PLAYER0;
    state.playerFolded[Player::PLAYER0] = true;
    assert(state.getUtility() == -2.0);
    
    // Test when opponent folded
    state.playerFolded[Player::PLAYER0] = false;
    state.playerFolded[Player::PLAYER1] = true;
    assert(state.getUtility() == 3.0);
    
    // Test when current player wins
    state.playerFolded[Player::PLAYER1] = false;
    state.playerIsWinner[Player::PLAYER0] = true;
    assert(state.getUtility() == 3.0);
    
    // Test when current player loses
    state.playerIsWinner[Player::PLAYER0] = false;
    state.playerIsWinner[Player::PLAYER1] = true;
    assert(state.getUtility() == -2.0);
    
    std::cout << "GetUtility test passed\n";
}

void test_getKey() {
    LeducState state;
    state.playerCards = {Card::JACK, Card::QUEEN};
    state.communityCard = Card::KING;
    state.actions = {{Action::FOLD}, {Action::CALL}};
    state.currentPlayer = Player::PLAYER0;
    
    long key = state.getKey();
    assert(key != 0);
    assert(key != -696969696); // Should not be the error value
    
    // Test with chance player (should return error value)
    state.currentPlayer = Player::CHANCE;
    key = state.getKey();
    assert(key == -696969696);
    
    std::cout << "GetKey test passed.\n";
}

void test_complete_game_scenarios() {
    // Test scenario 1: Player 0 folds
    LeducState state1;
    state1.applyChance(Chance::DEAL_JACK);
    state1.applyChance(Chance::DEAL_QUEEN);
    assert(state1.getCurrentPlayer() == Player::PLAYER0);
    state1.applyAction(Action::FOLD);
    assert(state1.isTerminalNode);
    assert(state1.playerFolded[Player::PLAYER0]);
    assert(state1.getUtility() == -1.0); // Player 0 loses their ante
    
    // Test scenario 2: Check-check in round 0, then showdown
    LeducState state2;
    state2.applyChance(Chance::DEAL_JACK);
    state2.applyChance(Chance::DEAL_QUEEN);
    state2.applyAction(Action::CALL); // Player 0 checks
    state2.applyAction(Action::CALL); // Player 1 checks
    assert(state2.getCurrentPlayer() == Player::CHANCE);
    assert(state2.round == 1);
    state2.applyChance(Chance::DEAL_KING);
    state2.applyAction(Action::CALL); // Player 0 checks
    state2.applyAction(Action::CALL); // Player 1 checks
    assert(state2.isTerminalNode);
    assert(!state2.playerFolded[0] && !state2.playerFolded[1]);
    assert(state2.playerIsWinner[1]); // QUEEN > JACK
    
    // Test scenario 3: Raise-call in round 0, then showdown
    LeducState state3;
    state3.applyChance(Chance::DEAL_JACK);
    state3.applyChance(Chance::DEAL_QUEEN);
    state3.applyAction(Action::RAISE); // Player 0 raises to 3
    assert(state3.playerAnte[Player::PLAYER0] == 3.0);
    state3.applyAction(Action::CALL); // Player 1 calls
    assert(state3.playerAnte[Player::PLAYER1] == 3.0);
    assert(state3.getCurrentPlayer() == Player::CHANCE);
    state3.applyChance(Chance::DEAL_JACK);
    state3.applyAction(Action::CALL); // Player 0 checks
    state3.applyAction(Action::CALL); // Player 1 checks
    assert(state3.isTerminalNode);
    assert(state3.playerIsWinner[0]); // Player 0 has pair
    
    std::cout << "Complete game scenarios test passed\n";
}

void test_betting_structure() {
    // Test round 0 betting structure (1 chip bets)
    LeducState state1;
    state1.applyChance(Chance::DEAL_JACK);
    state1.applyChance(Chance::DEAL_QUEEN);
    assert(state1.playerAnte[0] == 1.0 && state1.playerAnte[1] == 1.0);
    
    // First raise should be 2 chips
    state1.applyAction(Action::RAISE);
    assert(state1.playerAnte[Player::PLAYER0] == 3.0); // 1 + 2
    
    // Second raise should be 2 more chips
    state1.applyAction(Action::RAISE);
    assert(state1.playerAnte[Player::PLAYER1] == 5.0); // 3 + 2
    
    // Test round 1 betting structure (4 chip bets)
    LeducState state2;
    state2.round = 1;
    state2.playerAnte = {3.0, 3.0}; // After round 0
    state2.actions = {{Action::CALL, Action::CALL}, {}};
    state2.playerCards = {Card::JACK, Card::QUEEN};
    
    // First raise should be 4 chips
    state2.applyAction(Action::RAISE);
    assert(state2.playerAnte[Player::PLAYER0] == 7.0); // 3 + 4
    
    // Second raise should be 4 more chips
    state2.applyAction(Action::RAISE);
    assert(state2.playerAnte[Player::PLAYER1] == 11.0); // 7 + 4
    
    std::cout << "Betting structure test passed\n";
}

int main() {
    test_constructor();
    test_clone();
    test_legalActions();
    test_applyAction_fold();
    test_applyAction_call_round0();
    test_applyAction_call_round1();
    test_applyAction_raise_round0();
    test_applyAction_raise_round1();
    test_applyAction_raise_limit();
    test_chanceOutcomes();
    test_applyChance_round0();
    test_applyChance_round1_winning_logic();
    test_getUtility();
    test_getKey();
    test_complete_game_scenarios();
    test_betting_structure();
    
    std::cout << "All LeducState tests passed!\n";
    return 0;
}