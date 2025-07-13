#include "KuhnState.h"
#include <cassert>

KuhnState::KuhnState() : GameState(2, -1) {
	isChanceState = true;
	isTerminalState = false;
	playerAnte = {1.0, 1.0};
	playerIsWinner = {false, false};
	playerFolded = {false, false};
	deck = {JACK, QUEEN, KING};
}

KuhnState::~KuhnState() {
}

bool KuhnState::isTerminal() const {
	return isTerminalState;
}

bool KuhnState::isChance() const {
	return isChanceState;
}

double KuhnState::getUtility(int player) const {
	assert(isTerminalState && (player == 0 || player == 1) && (currentPlayer == 0 || currentPlayer == 1));

	if (playerFolded[player]) {
		return -playerAnte[player];
	}

	if (playerFolded[1 - player]) {
		return playerAnte[1 - player];
	}

	if (playerIsWinner[player]) {
		return playerAnte[1 - player];
	}

	return -playerAnte[player];
}

std::vector<Action> KuhnState::getLegalActions() const {
	assert(!isTerminalState && (currentPlayer == 0 || currentPlayer == 1));
	return {Action(Action::Type::Fold), Action(Action::Type::Raise)};
}

std::unordered_map<Chance, double> KuhnState::getChance() const {
	assert(isChanceState && currentPlayer == -1);
	std::unordered_map<Chance, double> outcomes;

	for (int card : deck) {
		outcomes[Chance(card)] = 1.0 / static_cast<int>(deck.size());
	}

	return outcomes;
}

void KuhnState::applyAction(Action action) {
	assert(!isTerminalState && actions.size() <= 3 && (currentPlayer == 0 || currentPlayer == 1));

	int actionCount = actions.size();
	Action lastAction = Action(Action::Type::Invalid);

	if (actionCount > 0) {
		lastAction = actions[actionCount - 1];
	}

	// std::cout << "lastAction: " << getActionIndex(lastAction) << std::endl;
	// std::cout << "action: " << getActionIndex(action) << std::endl;

	if (action.type == Action::Type::Fold) {
		if (lastAction.type == Action::Type::Raise) { // currentPlayer folds
			isTerminalState = true;
			playerFolded[currentPlayer] = true;
		} else if (lastAction.type == Action::Type::Fold) { // showdown
			isTerminalState = true;
		}
	} else if (action.type == Action::Type::Raise) {
		++playerAnte[currentPlayer];
		if (lastAction.type == Action::Type::Raise) { // showdown
			isTerminalState = true;
		}
	} else {
		std::cout << "Invalid action" << std::endl;
		return;
	}

	actions.push_back(action);
	currentPlayer = 1 - currentPlayer;
}

void KuhnState::applyChance(Chance chance) {
	assert(currentPlayer == -1);
	int card = chance.value;

	playerCards.push_back(card);

	chances.push_back(chance);

	std::vector<int> newDeck;

	for (int i = 0; i < deck.size(); ++i) {
		if (deck[i] != card) {
			newDeck.push_back(deck[i]);
		}
	}

	deck = newDeck;

	if (chances.size() >= 2) {
		currentPlayer = 0;
		isChanceState = false;
		playerIsWinner[0] = playerCards[0] > playerCards[1];
		playerIsWinner[1] = playerCards[0] < playerCards[1];
	}
}

long KuhnState::getKey() const {
	assert(currentPlayer != -1);
	long key = 7.0;

	for (Action a : actions) {
		key = key * 10 + getActionIndex(a);
	}

	return key * 10000 + 7000 + currentPlayer * 100 + 70 + playerCards[currentPlayer];
}

int KuhnState::getActionIndex(Action action) const {
	if (action.type == Action::Type::Fold) {
		return 0;
	} else if (action.type == Action::Type::Raise) {
		return 1;
	}
	return -1; // error, bad action
}

std::unique_ptr<GameState> KuhnState::clone() const {
	std::unique_ptr<KuhnState> newState = std::make_unique<KuhnState>();
	newState->currentPlayer = currentPlayer;
	newState->actionCount = actionCount;
	newState->playerCards = playerCards;
	newState->deck = deck;
	newState->chances = chances;
	newState->playerIsWinner = playerIsWinner;
	newState->playerFolded = playerFolded;
	newState->playerAnte = playerAnte;
	newState->isTerminalState = isTerminalState;
	newState->isChanceState = isChanceState;
	newState->actions = actions;
	return newState;
}