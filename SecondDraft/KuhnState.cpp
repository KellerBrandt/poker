#include "KuhnState.h"
#include "GameEnums.h"

//initializes root node
KuhnState::KuhnState() {
    currentPlayer = Player::CHANCE;
    playerCards = std::vector<Card>(2, Card::NULL_CARD);
    pot = 0;
    playerAnte = std::vector<int>(2, 0);
    actions = std::vector<Action>(2, Action::NULL_ACTION);
    playerFolded = std::vector<bool>(2, false);
    playerIsWinner = std::vector<bool>(2, false);
}

Player KuhnState::getCurrentPlayer() const {
    return currentPlayer;
}

bool KuhnState::isTerminal() const {
    int history = 0;
    for (Action a : actions) {
        history = history * 10 + a;
    }
    return history == 11 || history == 21 || history == 22 || history == 121 || history == 122;
}

bool KuhnState::isChance() const {
    for (auto& a : actions) {
        if (a == Action::NULL_ACTION) {
            return true;
        }
    }
    return false;
}

double KuhnState::getUtility(Player player) const {}

std::vector<Action> KuhnState::legalActions() const {}

std::unique_ptr<GameState> KuhnState::clone() const {}

void KuhnState::applyAction(Action action) {}

void KuhnState::applyChance(Chance chance) {}

std::vector<std::pair<Chance, double>> KuhnState::chanceOutcomes() const {}

Player KuhnState::getKey(Player player) const {}