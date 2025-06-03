#include "CFR.h"
#include <iostream>

void CFR::runVanillaCFR(std::unique_ptr<GameState>& root, int iterations) {
    double sum = 0.0;
    for (int i = 0; i < iterations; i++) {
        sum += vanillaCFR(root, 1.0, 1.0, 1.0);
    }
    std::cout << "Average EV: " << sum / iterations << std::endl;
}

double CFR::vanillaCFR(std::unique_ptr<GameState>& state, double p0, double p1, double pChance) {
    if (state->isTerminal()) {
        return state->getUtility(state->getCurrentPlayer());
    }

    Player currentPlayer = state->getCurrentPlayer();

    if (currentPlayer == Player::CHANCE) { //might handle probabilities wrong here with duplicate deals to p0 and p1
        std::vector<std::pair<Chance, double>> chanceOutcomes = state->chanceOutcomes();
        double sum = 0.0;
        for (auto& outcome : chanceOutcomes) {
            auto nextState = state->clone();
            nextState->applyChance(outcome.first);
            sum += outcome.second * vanillaCFR(nextState, p0, p1, pChance * outcome.second); //maybe remove pChance and multiply cfr by chanceOutcomes[i].second
        }
        return sum;
    }

    int key = state->getKey(state->getCurrentPlayer());

    if (infosets.find(key) == infosets.end()) {
        infosets[key] = InfoSet(Action::ACTION_COUNT);
    }

    InfoSet& infoSet = infosets[key];

    std::vector<double> strategy = infoSet.getStrategy();
    
    double currentStateEV = 0.0;
    std::vector<double> actionsEV(Action::ACTION_COUNT, 0.0);

    double pOpponent = (currentPlayer == Player::PLAYER0) ? p1 : p0;

    //maybe change this part, I dislike how actions are treated as indices
    if (currentPlayer == Player::PLAYER0) {
        for (auto& action : state->legalActions()) {
            auto nextState = state->clone();
            nextState->applyAction(action);
            actionsEV[action] = -vanillaCFR(nextState, p0 * strategy[action], p1, pChance);
            currentStateEV += strategy[action] * actionsEV[action];
        }
    } else if (currentPlayer == Player::PLAYER1) {
        for (auto& action : state->legalActions()) {
            auto nextState = state->clone();
            nextState->applyAction(action);
            actionsEV[action] = -vanillaCFR(nextState, p0, p1 * strategy[action], pChance);
            currentStateEV += strategy[action] * actionsEV[action];
        }
    } else {
        std::cout << "Error: Invalid player" << std::endl;
        return -1;
    }
    
    std::vector<double> actionCFR(Action::ACTION_COUNT, 0.0);

    for (auto& action : state->legalActions()) {
        actionCFR[action] = pChance * pOpponent * (actionsEV[action] - currentStateEV);
    }

    infoSet.updateRegretSum(actionCFR);
    infoSet.updateStrategySum(strategy);

    return currentStateEV;
}