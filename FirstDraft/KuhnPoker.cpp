#include "KuhnPoker.h"
#include <cmath>

KuhnPoker::KuhnPoker() : Game(2, 3) {
    playerCardStates[0] = {0, 1};
    playerCardStates[1] = {0, 2};
    playerCardStates[2] = {1, 0};
    playerCardStates[3] = {1, 2};
    playerCardStates[4] = {2, 0};
    playerCardStates[5] = {2, 1};

    actions[0] = 1;
    actions[1] = 2;
};

int KuhnPoker::getKey(int currentPlayer, int history, int state) {
    return history * 100 + currentPlayer * 10 + playerCardStates[state][currentPlayer];
}

bool KuhnPoker::isTerminal(int history) {
    return history == 3311 || history == 3321 || history == 3322 || history == 33121 || history == 33122;
}

int KuhnPoker::getCurrentPlayer(int history) {
    if (history < 33) {
        std::cout << "Cannot get current player of chance state " << history << std::endl;
        return -1;
    }
    return (static_cast<int>(std::log10(std::abs(history))) + 1) % 2;
}

double KuhnPoker::getUtility(int history, int currentPlayer, int state) {
    if (!isTerminal(history)) {
        std::cout << "Cannot get utility of non terminal state" << std::endl;
        return -1;
    }
    if (history == 3311) {
        return (playerCardStates[state][currentPlayer] > playerCardStates[state][!currentPlayer]) ? 1 : -1;
    }
    if (history == 3321) {
        return 1;
    }
    if (history == 3322) {
        return (playerCardStates[state][currentPlayer] > playerCardStates[state][!currentPlayer]) ? 2 : -2;
    }
    if (history == 33121) {
        return 1;
    }
    if (history == 33122) {
        return (playerCardStates[state][currentPlayer] > playerCardStates[state][!currentPlayer]) ? 2 : -2;
    }
    std::cout << "Error" << std::endl;
    return -1;
}

void KuhnPoker::print() {
    for (auto &pair : infoSets) {
        std::cout << pair.second.toString() << std::endl;
    }
}