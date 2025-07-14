#include "LeducState.h"
#include <random>

/*
Fold
Call = check
Raise
*/

LeducState::LeducState() : GameState(3, -1),
						   round(0),
						   raiseCount(0),
						   publicCard(-1),
						   betAmount(0),
						   isTerminalState(false),
						   isChanceState(true),
						   chances{},
						   playerAnte{1.0, 1.0},
						   playerFolded{false, false},
						   playerWins{false, false},
						   playerCards{},
						   actions{{}, {}},
						   deck{2, 2, 2} {}

bool LeducState::isTerminal() const {
	return isTerminalState;
}

bool LeducState::isChance() const {
	return isChanceState;
}

double LeducState::getUtility(int player) const {
	assert(isTerminalState && playerCards.size() == 2 && currentPlayer != -1);

	if (playerFolded[player]) {
		return -playerAnte[player];
	}

	if (playerFolded[1 - player]) {
		return playerAnte[1 - player];
	}

	if (playerWins[player]) {
		return playerAnte[1 - player];
	}

	if (playerWins[1 - player]) {
		return -playerAnte[player];
	}

	if (playerCards[0] == playerCards[1]) {
		return 0; // tie
	}

	if (playerFolded[player] || playerFolded[1 - player] || playerWins[player] || playerWins[1 - player]) {
		std::cout << "how you even here muva fubba" << std::endl;
	}

	std::cout << "In utility" << std::endl;
	std::cout << "Public card: " << publicCard << ", P0 card: " << playerCards[0] << ", P1 card: " << playerCards[1] << std::endl;

	std::cout << "error muva fubba" << std::endl;

	assert(false);
}

std::vector<Action> noRaise = {Action(Action::Type::Fold), Action(Action::Type::Call)};
std::vector<Action> yesRaise = {Action(Action::Type::Fold), Action(Action::Type::Call), Action(Action::Type::Raise)};

std::vector<Action> LeducState::getLegalActions() const {
	if (raiseCount >= 2) {
		return noRaise;
	}
	return yesRaise;
}

std::unordered_map<Chance, double> LeducState::getChance() const {
	assert(isChanceState);

	int totalCards = 0;

	for (int i : deck) {
		totalCards += i;
	}

	std::unordered_map<Chance, double> outcomes;

	for (int i = 0; i < deck.size(); ++i) {
		if ((double)deck[i] > 0) {
			outcomes[Chance(i)] = (double)deck[i] / (double)totalCards;
		}
	}

	return outcomes; // check this
}

inline int betSize(int round) { return (round == 0) ? 2 : 4; }

void LeducState::applyAction(Action action) {
	assert(!isChanceState && !isTerminalState);

	Action lastAction = Action(Action::Type::Empty);

	if (actions[round].size() > 0) {
		lastAction = actions[round][actions[round].size() - 1];
	}

	actions[round].push_back(action);

	if (action.type == Action::Type::Fold) {
		playerFolded[currentPlayer] = true;
		isTerminalState = true;
	} else if (action.type == Action::Type::Call) {
		if (lastAction.type == Action::Type::Empty) {
			// first action of round, do players switched later
		} else if (lastAction.type == Action::Type::Call) {
			++round;
			isChanceState = true;
		} else if (lastAction.type == Action::Type::Raise) {
			playerAnte[currentPlayer] += betSize(round);

			++round;
			isChanceState = true;
		} else {
			std::cout << "invalid last action" << std::endl;
		}
	} else if (action.type == Action::Type::Raise) {
		++raiseCount;
		playerAnte[currentPlayer] += betSize(round);
		if (lastAction.type == Action::Type::Raise) {
			playerAnte[currentPlayer] += betSize(round);
		}
	} else {
		std::cout << "invalid action" << std::endl;
	}

	if (round > 1) {
		isTerminalState = true;
	}

	currentPlayer = 1 - currentPlayer;
}

void LeducState::revertAction(Action action) {
}

void LeducState::applyChance(Chance chance) {
	assert(isChanceState && (deck[chance.value] > 0) && chances.size() <= 2);

	chances.push_back(chance);

	if (round == 0) {
		playerCards.push_back(chance.value);
		--deck[chance.value];
		if (chances.size() >= 2 || playerCards.size() >= 2) {
			isChanceState = false;
			currentPlayer = 0;
		}
	} else if (round == 1) {
		publicCard = chance.value;
		isChanceState = false;
		currentPlayer = 0;
		raiseCount = 0;

		if (playerCards[0] == publicCard) {
			playerWins[0] = true;
			playerWins[1] = false;
		} else if (playerCards[1] == publicCard) {
			playerWins[0] = false;
			playerWins[1] = true;
		} else if (playerCards[0] == playerCards[1]) {
			playerWins[0] = false;
			playerWins[1] = false;
		} else {
			playerWins[0] = playerCards[0] > playerCards[1];
			playerWins[1] = playerCards[0] < playerCards[1];
		}
	}
}

void LeducState::revertChance(Chance chance) {}

long LeducState::getKey() const {
	assert(!isChanceState && currentPlayer != -1);

	long key = 7;

	for (const auto &round : actions) {
		for (const auto &action : round) {
			key = key * 10 + getActionIndex(action);
		}
		key = key * 10 + 7;
	}

	if (round == 0) {
		return key * 100 + currentPlayer * 10 + playerCards[currentPlayer];
	} else if (round == 1) {
		return key * 1000 + publicCard * 100 + currentPlayer * 10 + playerCards[currentPlayer];
	}
	return -1; // invalid
}

int LeducState::getActionIndex(Action action) const {
	return (int)action.type;
}

std::unique_ptr<GameState> LeducState::clone() const {
	return std::make_unique<LeducState>(*this);
}