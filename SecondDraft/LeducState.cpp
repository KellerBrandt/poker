#include "LeducState.h"

LeducState::LeducState() {}

Player LeducState::getCurrentPlayer() const {
    return currentPlayer;
}

bool LeducState::isTerminal() const {
	return isTerminalNode;
}

bool LeducState::isChance() const {
	return currentPlayer == Player::CHANCE;
}

double LeducState::getUtility() const {
	if (playerFolded[currentPlayer]) {
		return -playerAnte[currentPlayer];
	} else if (playerFolded[1 - currentPlayer]) {
		return playerAnte[1 - currentPlayer];
	}
	return playerIsWinner[currentPlayer] ? playerAnte[1 - currentPlayer] : -playerAnte[currentPlayer];
}

std::vector<Action> LeducState::legalActions() const {
	
}

std::unique_ptr<GameState> LeducState::clone() const {

}

void LeducState::applyAction(Action action) {

}

void LeducState::applyChance(Chance chance) {

}

std::vector<std::pair<Chance, double>> LeducState::chanceOutcomes() const {

}

int LeducState::getKey() const {

}

void LeducState::print() const {}