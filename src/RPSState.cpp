#include "RPSState.h"

RPSState::RPSState() : GameState(3, 0) {}

RPSState::~RPSState() {}

bool RPSState::isTerminal() const {
    return actions.size() > 1;
}

bool RPSState::isChance() const {
    return false;
}

double RPSState::getUtility(int player) const {
    if (actions.size() < 2) {
        std::cout << "shouldnt be here" << std::endl;
        return 0.0;
    }
    const Action& a0 = actions[0];
    const Action& a1 = actions[1];

    bool p0wins = false;

    if (a0.type == a1.type) {
        return 0.0;
    }

    if (a0.type == Action::Type::Rock && a1.type == Action::Type::Scissors) {
        p0wins = true;
    } else if (a0.type == Action::Type::Paper && a1.type == Action::Type::Rock) {
        p0wins = true;
    } else if (a0.type == Action::Type::Scissors && a1.type == Action::Type::Paper) {
        p0wins = true;
    }

    if (p0wins) {
        return player == 0 ? 1.0 : -1.0;
    } else {
        return player == 0 ? -1.0 : 1.0;
    }
}

std::vector<Action> RPSState::getLegalActions() const {
    return {Action(Action::Type::Rock), Action(Action::Type::Paper), Action(Action::Type::Scissors)};
}

void RPSState::applyAction(Action action) {
    if (isTerminal()) {
        std::cout << "error" << std::endl;
    }
    actions.push_back(action);
    currentPlayer = 1 - currentPlayer;
}

void RPSState::revertAction(Action action) {

}

void RPSState::applyChance(Chance chance) {
	return;
}

void RPSState::revertChance(Chance chance) {

}

std::unordered_map<Chance, double> RPSState::getChance() const {
	return {};
}

long RPSState::getKey() const {
    return currentPlayer;
}

int RPSState::getActionIndex(Action action) const {
    if (action.type == Action::Type::Rock) {
        return 0;
    } else if (action.type == Action::Type::Paper) {
        return 1;
    } else if (action.type == Action::Type::Scissors) {
        return 2;
    }
    std::cout << "why are you here???" << std::endl;
    return -1;
}

std::unique_ptr<GameState> RPSState::clone() const {
	std::unique_ptr<RPSState> newState = std::make_unique<RPSState>();
	newState->currentPlayer = currentPlayer;
	newState->actionCount = actionCount;
	newState->actions = actions;
	return newState;
}