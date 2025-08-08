#include "KuhnState.h"
#include <algorithm>

KuhnState::KuhnState()
	: GameState(3, -1),
	  raisesThisRound(0),
	  toCall(0),
	  isTerminalState(false),
	  isChanceState(true),
	  deckCounts{1, 1, 1},
	  chances{},
	  playerCards{},
	  actions{},
	  history{},
	  playerAnte{1.0, 1.0},
	  playerFolded{false, false} {}

bool KuhnState::isTerminal() const {
	return isTerminalState;
}

bool KuhnState::isChance() const {
	return isChanceState;
}

double KuhnState::getUtility(int player) const {
	assert(isTerminalState);
	if (playerFolded[player]) return -playerAnte[player];
	if (playerFolded[1 - player]) return playerAnte[1 - player];
	// showdown (distinct ranks in Kuhn)
	if (playerCards[0] > playerCards[1])
		return (player == 0) ? playerAnte[1] : -playerAnte[0];
	else
		return (player == 1) ? playerAnte[0] : -playerAnte[1];
}

std::vector<Action> KuhnState::getLegalActions() const {
	std::vector<Action> legal;
	legal.emplace_back(Action::Type::Fold); // allowed always for symmetry with LeducState
	legal.emplace_back(Action::Type::Call); // check/call
	if (raisesThisRound < 1) legal.emplace_back(Action::Type::Raise);
	return legal;
}

std::unordered_map<Chance, double> KuhnState::getChance() const {
	assert(isChanceState);
	int total = deckCounts[0] + deckCounts[1] + deckCounts[2];
	std::unordered_map<Chance, double> outcomes;
	for (int r = 0; r < 3; ++r)
		if (deckCounts[r] > 0)
			outcomes[Chance(r)] = double(deckCounts[r]) / double(total);
	return outcomes;
}

void KuhnState::applyAction(Action action) {
	assert(!isChanceState && !isTerminalState && currentPlayer >= 0);

	ActionRecord rec{};
	rec.action = action;
	rec.prevCurrentPlayer = currentPlayer;
	rec.prevIsTerminal = isTerminalState;
	rec.prevIsChance = isChanceState;
	rec.prevRaises = raisesThisRound;
	rec.prevToCall = toCall;
	rec.prevFoldP0 = playerFolded[0];
	rec.prevFoldP1 = playerFolded[1];
	rec.anteDeltaP0 = 0.0;
	rec.anteDeltaP1 = 0.0;

	int bs = betSize();

	if (action.type == Action::Type::Fold) {
		playerFolded[currentPlayer] = true;
		isTerminalState = true;
		actions.push_back(action);
		currentPlayer = 1 - currentPlayer;
		history.push_back(rec);
		return;
	}

	if (action.type == Action::Type::Call) {
		if (toCall > 0) {
			if (currentPlayer == 0) {
				playerAnte[0] += toCall;
				rec.anteDeltaP0 = toCall;
			} else {
				playerAnte[1] += toCall;
				rec.anteDeltaP1 = toCall;
			}
			toCall = 0;
			isTerminalState = true; // calling ends the (only) round
			currentPlayer = 1 - currentPlayer;
		} else {
			// check
			if (!actions.empty() && actions.back().type == Action::Type::Call) {
				isTerminalState = true; // two checks -> showdown
				currentPlayer = 1 - currentPlayer;
			} else {
				currentPlayer = 1 - currentPlayer;
			}
		}
		actions.push_back(action);
		history.push_back(rec);
		return;
	}

	if (action.type == Action::Type::Raise) {
		int add = rec.prevToCall + bs; // complete any call + new bet
		if (currentPlayer == 0) {
			playerAnte[0] += add;
			rec.anteDeltaP0 = add;
		} else {
			playerAnte[1] += add;
			rec.anteDeltaP1 = add;
		}
		toCall = bs;
		raisesThisRound += 1;
		actions.push_back(action);
		currentPlayer = 1 - currentPlayer;
		history.push_back(rec);
		return;
	}
}

void KuhnState::revertAction(Action action) {
	(void)action;
	assert(!history.empty());
	ActionRecord rec = history.back();
	history.pop_back();
	assert(!actions.empty());
	actions.pop_back();

	currentPlayer = rec.prevCurrentPlayer;
	isTerminalState = rec.prevIsTerminal;
	isChanceState = rec.prevIsChance;
	raisesThisRound = rec.prevRaises;
	toCall = rec.prevToCall;
	playerFolded[0] = rec.prevFoldP0;
	playerFolded[1] = rec.prevFoldP1;
	playerAnte[0] -= rec.anteDeltaP0;
	playerAnte[1] -= rec.anteDeltaP1;
}

void KuhnState::applyChance(Chance chance) {
	assert(isChanceState);
	int r = chance.value;
	assert(r >= 0 && r <= 2 && deckCounts[r] > 0);
	deckCounts[r] -= 1;
	chances.push_back(chance);

	playerCards.push_back(r);
	if (playerCards.size() == 2) {
		isChanceState = false;
		currentPlayer = 0;
		raisesThisRound = 0;
		toCall = 0;
	}
}

void KuhnState::revertChance(Chance chance) {
	int r = chance.value;
	assert(deckCounts[r] >= 0);
	deckCounts[r] += 1;
	assert(!chances.empty());
	chances.pop_back();
	assert(!playerCards.empty());
	playerCards.pop_back();

	isChanceState = true;
	currentPlayer = -1;
	raisesThisRound = 0;
	toCall = 0;
}

long KuhnState::getKey() const {
	assert(!isChanceState && currentPlayer != -1 && playerCards.size() == 2);
	long key = 7;
	for (const auto &a : actions)
		key = key * 10 + getActionIndex(a);
	key = key * 10 + 7;
	return key * 100 + currentPlayer * 10 + playerCards[currentPlayer];
}

int KuhnState::getActionIndex(Action action) const {
	if (action.type == Action::Type::Fold) return 0;
	if (action.type == Action::Type::Call) return 1;
	if (action.type == Action::Type::Raise) return 2;
	return -1;
}

std::unique_ptr<GameState> KuhnState::clone() const {
	return std::make_unique<KuhnState>(*this);
}