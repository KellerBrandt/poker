#include "LeducState.h"

LeducState::LeducState() : GameState(3, 0) {

}

bool LeducState::isTerminal() const {
    return isTerminalState;
}

bool LeducState::isChance() const {
    return isChanceState;
}

double LeducState::getUtility(int player) const {
    if (playerFolded[player]) {
		return -playerAnte[player];
	}

	if (playerFolded[1 - player]) {
		return playerAnte[1 - player];
	}

	if (playerIsWinner[player]) {
		return playerAnte[1 - player];
	}

    if (playerIsWinner[1 - player]) {
        return -playerAnte[player];
    }

	return 0; // tie
}

std::vector<Action> LeducState::getLegalActions() const {
}

std::unordered_map<Chance, double> LeducState::getChance() const {
}

void LeducState::applyAction(Action action) {
}

void LeducState::applyChance(Chance chance) {
}

long LeducState::getKey() const {
}

int LeducState::getActionIndex(Action action) const {
}

std::unique_ptr<GameState> LeducState::clone() const {
}
