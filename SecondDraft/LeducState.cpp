#include "LeducState.h"

LeducState::LeducState() {
    round = 0;
    roundRaiseCount = 0;
    isTerminalNode = false;
    currentPlayer = Player::CHANCE;
    deck = {Card::JACK, Card::JACK, Card::QUEEN, Card::QUEEN, Card::KING, Card::KING};
    playerAnte = {1.0, 1.0};
    actions = {{}, {}};
    playerFolded = {false, false};
    playerIsWinner = {false, false};
}

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
    if (isTerminalNode || currentPlayer == Player::CHANCE) {
        std::cout << "Terminal or chance node should not call legalActions" << std::endl;
        return {};
    }

    if (roundRaiseCount > 1) {
        return {Action::FOLD, Action::CALL};
    }

    return {Action::FOLD, Action::CALL, Action::RAISE};
}

std::unique_ptr<GameState> LeducState::clone() const {
    auto clonedState = std::make_unique<LeducState>();
    clonedState->round = this->round;
    clonedState->roundRaiseCount = this->roundRaiseCount;
    clonedState->isTerminalNode = this->isTerminalNode;
    clonedState->currentPlayer = this->currentPlayer;
    clonedState->playerCards = this->playerCards;
    clonedState->deck = this->deck;
    clonedState->communityCard = this->communityCard;
    clonedState->playerAnte = this->playerAnte;
    clonedState->actions = this->actions;
    clonedState->chances = this->chances;
    clonedState->playerFolded = this->playerFolded;
    clonedState->playerIsWinner = this->playerIsWinner;
    return clonedState;
}

void LeducState::applyAction(Action action) {
    // Ensure round is within bounds
    if (round >= 2) {
        std::cout << "Error: Round " << round << " is out of bounds for actions vector" << std::endl;
        return;
    }

    int actionCount = actions[round].size();
    Action lastAction = Action::NULL_ACTION;

    if (actionCount > 0) {
        lastAction = actions[round][actionCount - 1];
    }

    actions[round].push_back(action);

    if (action == Action::FOLD) {
        playerFolded[currentPlayer] = true;
        isTerminalNode = true;
        currentPlayer = (currentPlayer == Player::PLAYER0) ? Player::PLAYER1 : Player::PLAYER0;
        return;
    } else if (action == Action::CALL) {
        if (lastAction != Action::NULL_ACTION && (lastAction == Action::CALL || lastAction == Action::RAISE)) {
            // if here, current round is over
            if (lastAction == Action::RAISE) {
                if (round == 0) {
                    playerAnte[currentPlayer] = playerAnte[1 - currentPlayer];
                } else if (round == 1) {
                    playerAnte[currentPlayer] = playerAnte[1 - currentPlayer];
                } else {
                    std::cout << "Invalid round" << std::endl;
                }
            }

            if (round == 0) {
                currentPlayer = Player::CHANCE;
            } else if (round == 1) {
                currentPlayer = (currentPlayer == Player::PLAYER0) ? Player::PLAYER1 : Player::PLAYER0;
                isTerminalNode = true;
            }

            ++round;
            roundRaiseCount = 0;
        } else {
            // if here, currentAction was a call, and last action was NULL_ACTION, meaning it is player0 turn and they checked
            currentPlayer = (currentPlayer == Player::PLAYER0) ? Player::PLAYER1 : Player::PLAYER0;
        }
    } else if (action == Action::RAISE) {
        if (lastAction == Action::RAISE) {
            if (round == 0) {
                playerAnte[currentPlayer] = playerAnte[1 - currentPlayer] + 1;
            } else if (round == 1) {
                playerAnte[currentPlayer] = playerAnte[1 - currentPlayer] + 2;
            } else {
                std::cout << "Invalid round" << std::endl;
            }
        } else {
            if (round == 0) {
                playerAnte[currentPlayer] = playerAnte[1 - currentPlayer] + 1;
            } else if (round == 1) {
                playerAnte[currentPlayer] = playerAnte[1 - currentPlayer] + 2;
            } else {
                std::cout << "Invalid round" << std::endl;
            }
        }

        roundRaiseCount++; // Increment raise count
        currentPlayer = (currentPlayer == Player::PLAYER0) ? Player::PLAYER1 : Player::PLAYER0;
    } else {
        std::cout << "Invalid action" << std::endl;
    }

    if (round > 1) {
        isTerminalNode = true;
    }
}

Card chanceToCard(Chance chance) {
    if (chance == Chance::DEAL_JACK) {
        return Card::JACK;
    } else if (chance == Chance::DEAL_QUEEN) {
        return Card::QUEEN;
    } else if (chance == Chance::DEAL_KING) {
        return Card::KING;
    }
    return Card::NULL_CARD;
}

void LeducState::applyChance(Chance chance) {
    Card dealtCard = chanceToCard(chance);

    chances.push_back(chance);

    if (round == 0) {
        playerCards.push_back(dealtCard);

        // idk if this works, but it should remove the dealtCard
        auto it = std::find(deck.begin(), deck.end(), dealtCard);
        if (it != deck.end()) {
            deck.erase(it); // Remove the first occurrence
        }

        if (chances.size() == 2) {
            currentPlayer = Player::PLAYER0;
        }
    } else if (round == 1) {
        communityCard = dealtCard;

        // Ensure we have exactly 2 player cards before accessing them
        if (playerCards.size() != 2) {
            std::cout << "Error: Expected 2 player cards, got " << playerCards.size() << std::endl;
            return;
        }

        if (playerCards[0] == communityCard) {
            playerIsWinner[0] = true;
        } else if (playerCards[1] == communityCard) {
            playerIsWinner[1] = true;
        } else {
            playerIsWinner[0] = playerCards[0] > playerCards[1];
            playerIsWinner[1] = playerCards[0] < playerCards[1];
        }

        currentPlayer = Player::PLAYER0;
    } else {
        std::cout << "Invalid round number" << std::endl;
    }
}

// test with this implementation, then test where it doesn't include 0% chances
std::vector<std::pair<Chance, double>> LeducState::chanceOutcomes() const {
    std::vector<int> counts(3, 0); // Initialize with 3 zeros for JACK, QUEEN, KING
    int sum = 0;

    for (Card c : deck) {
        if (c >= 0 && c < 3) { // Only count valid cards
            ++counts[c];
            ++sum;
        }
    }

    if (sum == 0) {
        return {}; // No cards left in deck
    }

    return {std::make_pair(Chance::DEAL_JACK, (double)counts[0] / sum),
            std::make_pair(Chance::DEAL_QUEEN, (double)counts[1] / sum),
            std::make_pair(Chance::DEAL_KING, (double)counts[2] / sum)};
}

// might need review
long LeducState::getKey() const {
    long key = 7;

    for (size_t i = 0; i < actions.size(); ++i) {
        for (size_t j = 0; j < actions[i].size(); ++j) {
            key = 10 * key + actions[i][j];
        }
    }

    if (currentPlayer == Player::CHANCE) {
        std::cout << "Error: key for chance" << std::endl;
        return -696969696;
    } else {
        return key * 10000 + 5000 + 100 * playerCards[currentPlayer] + 50 + communityCard;
    }
}

void LeducState::print() const {}

// GET RID OF CHANCE_ACTION
// want ev of -0.08553