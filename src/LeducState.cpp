#include "LeducState.h"
#include <algorithm>
#include <iostream>

LeducState::LeducState()
	: GameState(3, -1), 
	  round(0), 
	  raisesThisRound(0), 
	  toCall(0), 
	  publicCard(-1),
	  isTerminalState(false), 
	  isChanceState(true),
	  deckCounts{2, 2, 2}, 
	  chances{}, 
	  playerCards{},
	  actionsRound0{}, 
	  actionsRound1{}, 
	  history{},
	  playerAnte{1.0, 1.0}, 
	  playerFolded{false, false} {}

bool LeducState::isTerminal() const {
	return isTerminalState;
}

bool LeducState::isChance() const {
	return isChanceState;
}

// Returns -1 for tie, 0/1 for winner
int LeducState::winner() const {
	// Pair with public card > high card.
	bool p0pair = (publicCard != -1) && (playerCards[0] == publicCard);
	bool p1pair = (publicCard != -1) && (playerCards[1] == publicCard);

	if (p0pair && !p1pair) return 0;
	if (p1pair && !p0pair) return 1;
	if (!p0pair && !p1pair) {
		if (playerCards[0] > playerCards[1]) return 0;
		if (playerCards[1] > playerCards[0]) return 1;
		return -1;
	}
	// both paired is impossible in Leduc (only 2 copies of each rank)
	return -1;
}

double LeducState::getUtility(int player) const {
	assert(isTerminalState && player >= 0 && player <= 1);

	if (playerFolded[player]) {
		return -playerAnte[player];
	}
	if (playerFolded[1 - player]) {
		return playerAnte[1 - player];
	}
	// showdown
	int w = winner();
	if (w == -1) return 0.0;
	return (w == player) ? playerAnte[1 - player] : -playerAnte[player];
}

std::vector<Action> LeducState::getLegalActions() const {
	// Always allow fold (even though folding facing no bet is dominated; keeps things simple).
	std::vector<Action> legal{Action(Action::Type::Fold)};

	// Call (check) is always legal.
	legal.push_back(Action(Action::Type::Call));

	// Raise allowed up to 2 raises per round.
	if (raisesThisRound < 2) {
		legal.push_back(Action(Action::Type::Raise));
	}
	return legal;
}

std::unordered_map<Chance, double> LeducState::getChance() const {
	assert(isChanceState);

	int total = deckCounts[0] + deckCounts[1] + deckCounts[2];
	std::unordered_map<Chance, double> outcomes;
	for (int r = 0; r < 3; ++r) {
		if (deckCounts[r] > 0) {
			outcomes[Chance(r)] = static_cast<double>(deckCounts[r]) / static_cast<double>(total);
		}
	}
	return outcomes;
}

void LeducState::applyAction(Action action) {
	assert(!isTerminalState && !isChanceState && currentPlayer >= 0 && currentPlayer <= 1);

	ActionRecord rec{};
	rec.action = action;
	rec.prevCurrentPlayer = currentPlayer;
	rec.prevIsTerminal = isTerminalState;
	rec.prevIsChance = isChanceState;
	rec.prevRound = round;
	rec.prevRaises = raisesThisRound;
	rec.prevToCall = toCall;
	rec.prevPublicCard = publicCard;
	rec.prevFoldP0 = playerFolded[0];
	rec.prevFoldP1 = playerFolded[1];
	rec.didAdvanceRound = false;
	rec.didTerminate = false;
	rec.anteDeltaP0 = 0.0;
	rec.anteDeltaP1 = 0.0;

	Action last = Action(Action::Type::Invalid);
	if (round == 0 && !actionsRound0.empty()) last = actionsRound0.back();
	if (round == 1 && !actionsRound1.empty()) last = actionsRound1.back();

	int bs = betSize();

	if (action.type == Action::Type::Fold) {
		playerFolded[currentPlayer] = true;
		isTerminalState = true;
		rec.didTerminate = true;
		if (round == 0)
			actionsRound0.push_back(action);
		else
			actionsRound1.push_back(action);
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
			if (round == 0) {
				round = 1;
				isChanceState = true;
				currentPlayer = -1;
				raisesThisRound = 0;
				rec.didAdvanceRound = true;
			} else {
				isTerminalState = true;
				rec.didTerminate = true;
				currentPlayer = 1 - currentPlayer;
			}
		} else {
			if (last.type == Action::Type::Call) {
				if (round == 0) {
					round = 1;
					isChanceState = true;
					currentPlayer = -1;
					raisesThisRound = 0;
					rec.didAdvanceRound = true;
				} else {
					isTerminalState = true;
					rec.didTerminate = true;
					currentPlayer = 1 - currentPlayer;
				}
			} else {
				currentPlayer = 1 - currentPlayer;
			}
		}

		if (rec.prevRound == 0)
			actionsRound0.push_back(action);
		else
			actionsRound1.push_back(action);
		history.push_back(rec);
		return;
	}

	if (action.type == Action::Type::Raise) {
		// amount added includes any call due plus the raise size
		int add = rec.prevToCall + bs;
		if (currentPlayer == 0) {
			playerAnte[0] += add;
			rec.anteDeltaP0 = add;
		} else {
			playerAnte[1] += add;
			rec.anteDeltaP1 = add;
		}

		toCall = bs;
		raisesThisRound += 1;

		if (rec.prevRound == 0)
			actionsRound0.push_back(action);
		else
			actionsRound1.push_back(action);

		currentPlayer = 1 - currentPlayer;
		history.push_back(rec);
		return;
	}

	std::cerr << "Invalid action in LeducState::applyAction\n";
}

void LeducState::revertAction(Action action) {
	(void)action; // we use our saved record
	assert(!history.empty());
	ActionRecord rec = history.back();
	history.pop_back();

	if (rec.prevRound == 0) {
		assert(!actionsRound0.empty());
		actionsRound0.pop_back();
	} else {
		assert(!actionsRound1.empty());
		actionsRound1.pop_back();
	}

	currentPlayer = rec.prevCurrentPlayer;
	isTerminalState = rec.prevIsTerminal;
	isChanceState = rec.prevIsChance;
	round = rec.prevRound;
	raisesThisRound = rec.prevRaises;
	toCall = rec.prevToCall;
	publicCard = rec.prevPublicCard;
	playerFolded[0] = rec.prevFoldP0;
	playerFolded[1] = rec.prevFoldP1;

	playerAnte[0] -= rec.anteDeltaP0;
	playerAnte[1] -= rec.anteDeltaP1;
}

void LeducState::applyChance(Chance chance) {
	assert(isChanceState);

	int r = chance.value;
	assert(r >= 0 && r <= 2 && deckCounts[r] > 0);
	deckCounts[r] -= 1;
	chances.push_back(chance);

	if (playerCards.size() < 2) {
		playerCards.push_back(r);
		if (playerCards.size() == 2) {
			isChanceState = false;
			currentPlayer = 0;
			round = 0;
			raisesThisRound = 0;
			toCall = 0;
		}
		return;
	}

	// deal public card
	publicCard = r;
	isChanceState = false;
	currentPlayer = 0;
	round = 1;
	raisesThisRound = 0;
	toCall = 0;
}

void LeducState::revertChance(Chance chance) {
	int r = chance.value;
	assert(deckCounts[r] >= 0);
	deckCounts[r] += 1;
	assert(!chances.empty());
	chances.pop_back();

	if (publicCard != -1 && r == publicCard && playerCards.size() == 2) {
		publicCard = -1;
		isChanceState = true;
		currentPlayer = -1;
		round = 1; // still waiting to deal the public card
		raisesThisRound = 0;
		toCall = 0;
		return;
	}

	// undo a private card
	assert(!playerCards.empty());
	playerCards.pop_back();
	isChanceState = true;
	currentPlayer = -1;
	raisesThisRound = 0;
	toCall = 0;
}

long LeducState::getKey() const {
	assert(!isChanceState && currentPlayer != -1 && playerCards.size() == 2);

	long key = 7;
	for (const auto &a : actionsRound0)
		key = key * 10 + getActionIndex(a);
	key = key * 10 + 7;

	if (publicCard == -1 || round == 0) {
		return key * 100 + currentPlayer * 10 + playerCards[currentPlayer];
	}

	for (const auto &a : actionsRound1)
		key = key * 10 + getActionIndex(a);
	key = key * 10 + 7;

	return key * 1000 + publicCard * 100 + currentPlayer * 10 + playerCards[currentPlayer];
}

int LeducState::getActionIndex(Action action) const {
	if (action.type == Action::Type::Fold) return 0;
	if (action.type == Action::Type::Call) return 1;
	if (action.type == Action::Type::Raise) return 2;
	return -1;
}

std::unique_ptr<GameState> LeducState::clone() const {
	return std::make_unique<LeducState>(*this);
}