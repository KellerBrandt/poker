#include "LeducState.h"

LeducState::LeducState() {}

Player LeducState::getCurrentPlayer() const {
    return currentPlayer;
}

bool LeducState::isTerminal() const {}

bool LeducState::isChance() const {}

double LeducState::getUtility(Player player) const {}

std::vector<Action> LeducState::legalActions() const {}

std::unique_ptr<GameState> LeducState::clone() const {}

void LeducState::applyAction(Action action) {}

void LeducState::applyChance(Chance chance) {}

std::vector<std::pair<Chance, double>> LeducState::chanceOutcomes() const {}

Player LeducState::getKey(Player player) const {}