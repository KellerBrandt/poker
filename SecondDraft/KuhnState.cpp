#include "KuhnState.h"
#include "GameEnums.h"
#include <algorithm>
#include <iostream>

// initializes root node
KuhnState::KuhnState() {
	isTerminalNode = false;
	currentPlayer = Player::CHANCE;
	deck = {Card::JACK, Card::QUEEN, Card::KING};
	playerAnte = {1.0, 1.0};
	playerFolded = {false, false};
	playerIsWinner = {false, false};
}

Player KuhnState::getCurrentPlayer() const {
	return currentPlayer;
}

bool KuhnState::isTerminal() const {
	return isTerminalNode;
}

bool KuhnState::isChance() const {
	return currentPlayer == Player::CHANCE;
}

double KuhnState::getUtility() const {
	if (playerFolded[currentPlayer]) {
		return -playerAnte[currentPlayer];
	} else if (playerFolded[1 - currentPlayer]) {
		return playerAnte[1 - currentPlayer];
	}
	return playerIsWinner[currentPlayer] ? playerAnte[1 - currentPlayer] : -playerAnte[currentPlayer];
}

std::vector<Action> KuhnState::legalActions() const {
	if (isTerminalNode || currentPlayer == Player::CHANCE) {
		std::cout << "Terminal or chance node should not call legalActions" << std::endl;
		return {};
	}
	return {Action::CHECK, Action::BET};
}

std::unique_ptr<GameState> KuhnState::clone() const {
	std::unique_ptr<KuhnState> newState = std::make_unique<KuhnState>();
	newState->isTerminalNode = isTerminalNode;
	newState->currentPlayer = currentPlayer;
	newState->playerCards = playerCards;
	newState->deck = deck;
	newState->playerAnte = playerAnte;
	newState->actions = actions;
	newState->chances = chances;
	newState->playerFolded = playerFolded;
	newState->playerIsWinner = playerIsWinner;
	return newState;
}

void KuhnState::applyAction(Action action) {
	int actionCount = actions.size();
	Action lastAction = NULL_ACTION;

	if (actionCount > 0) {
		lastAction = actions[actionCount - 1];
	}

	if (action == Action::CHECK) {
		if (lastAction == Action::BET) { //currentPlayer folds
			isTerminalNode = true;
			playerFolded[currentPlayer] = true;
		} else if (lastAction == Action::CHECK) { //showdown
			isTerminalNode = true;
		}
	} else if (action == Action::BET) {
		++playerAnte[currentPlayer];
		if (lastAction == Action::BET) { //showdown
			isTerminalNode = true;
		}
	} else {
		std::cout << "Invalid action" << std::endl;
		return;
	}

	actions.push_back(action);

	currentPlayer = (currentPlayer == Player::PLAYER0) ? Player::PLAYER1 : Player::PLAYER0;
}

Chance cardToChance(Card card) {
	if (card == Card::JACK) {
		return Chance::DEAL_JACK;
	} else if (card == Card::QUEEN) {
		return Chance::DEAL_QUEEN;
	} else if (card == Card::KING) {
		return Chance::DEAL_KING;
	}
	return Chance::NULL_CHANCE;
}

// should only be called when 0 or 1 chance event has occured, so only handles those
std::vector<std::pair<Chance, double>> KuhnState::chanceOutcomes() const {
	std::vector<std::pair<Chance, double>> outcomes;

	for (int i = 0; i < deck.size(); ++i) {
		outcomes.push_back(std::make_pair(cardToChance(deck[i]), 1.0 / deck.size()));
	}

	return outcomes;
}

Card chanceToCard(Chance chance) {
	if (chance == Chance::DEAL_JACK) {
		return Card::JACK;
	} else if (chance == Chance::DEAL_QUEEN) {
		return Card::QUEEN;
	} else if (chance == Chance::DEAL_KING) {
		return Card::KING;
	}
	return Card::NULL_CARD;
}

// sloppy, relies on chances having the same value as cards
void KuhnState::applyChance(Chance chance) {
	Card card = chanceToCard(chance);

	playerCards.push_back(card);

	chances.push_back(chance);

	std::vector<Card> newDeck;

	for (int i = 0; i < deck.size(); ++i) {
		if (deck[i] != card) {
			newDeck.push_back(deck[i]);
		}
	}

	deck = newDeck;

	if (chances.size() == 2) {
		currentPlayer = Player::PLAYER0;
		playerIsWinner[0] = playerCards[0] > playerCards[1];
		playerIsWinner[1] = playerCards[0] < playerCards[1];
	}
}

int KuhnState::getKey() const {
	int key = 7;

	for (int i = 0; i < actions.size(); ++i) {
		key = key * 10 + actions[i];
	}
	
	return key * 100 + 50 + playerCards[currentPlayer];
}

void KuhnState::print() const {}