#include "RPSState.h"
#include <memory>
#include <iostream>

RPSState::RPSState() {
	currentPlayer = Player::PLAYER0;
	isTerminalNode = false;
	actions = {Action::NULL_ACTION, Action::NULL_ACTION};
}

Player RPSState::getCurrentPlayer() const {
	return currentPlayer;
}

bool RPSState::isTerminal() const {
	return isTerminalNode;
}

bool RPSState::isChance() const {
	return false;
}

double RPSState::getUtility() const {
	bool p0Wins = false;
	if (actions[0] == Action::ROCK && actions[1] == Action::SCISSORS) {
		p0Wins = true;
	} else if (actions[0] == Action::PAPER && actions[1] == Action::ROCK) {
		p0Wins = true;
	} else if (actions[0] == Action::SCISSORS && actions[1] == Action::PAPER) {
		p0Wins = true;
	}

	if (actions[0] == actions[1]) {
		return 0;
	}
	
	if (currentPlayer == Player::PLAYER0) {
		return p0Wins ? 1 : -1;
	} else {
		return p0Wins ? -1 : 1;
	}
}

std::vector<Action> RPSState::legalActions() const {
	return {Action::ROCK, Action::PAPER, Action::SCISSORS};
}

std::unique_ptr<GameState> RPSState::clone() const {
	std::unique_ptr<RPSState> newState = std::make_unique<RPSState>();
	newState->currentPlayer = currentPlayer;
	newState->isTerminalNode = isTerminalNode;
	newState->actions = actions;
	return newState;
}

void RPSState::applyAction(Action action) {
	actions[currentPlayer] = action;
	if (currentPlayer == Player::PLAYER1) {
		currentPlayer = Player::PLAYER0;
		isTerminalNode = true;
	} else {
		currentPlayer = Player::PLAYER1;
	}
}

int RPSState::getKey() const {
	return actions[currentPlayer] * 100 + currentPlayer;
}

void RPSState::print() const {}

void RPSState::applyChance(Chance chance) {
	std::cout << "Error: RPSState is not a chance node" << std::endl;
}

std::vector<std::pair<Chance, double>> RPSState::chanceOutcomes() const {
	std::cout << "Error: RPSState is not a chance node" << std::endl;
	return {};
}