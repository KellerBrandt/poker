#include "CFR.h"

void CFR::runVanillaCFR(Game &game, int iterations) {
    //temporary, may have a starting state type of thing
    double sum = 0.0;
    double tempSum = 0.0;
    for (int i = 0; i < iterations; ++i) {
        for (int j = 0; j < game.playerCardStates.size(); ++j) {
            tempSum += vanillaCFR(game, j, 33, 1.0, 1.0, 1.0 / 6.0);
        }
        sum += tempSum / game.playerCardStates.size();
        tempSum = 0.0;
    }
    std::cout << "p0 EV: " << sum / iterations << std::endl;
}

//runs one iteration of cfr
double CFR::vanillaCFR(Game &game, int &state, int history, double p0, double p1, double pChance) {
    int currentPlayer = game.getCurrentPlayer(history);

    if (game.isTerminal(history)) {
        return game.getUtility(history, currentPlayer, state);
    }
    
    int key = game.getKey(currentPlayer, history, state);

    if (game.infoSets.find(key) == game.infoSets.end()) {
        game.infoSets[key] = InfoSet(game.actionCount, history, game.playerCardStates[state][currentPlayer]);
    }

    InfoSet &infoSet = game.infoSets[key];

    std::vector<double> strategy = infoSet.getStrategy();

    double currentEV = 0.0; //counterfactual value of the node given the current strategy; i.e. expected value of the node
    std::vector<double> actionsEV(game.actionCount, 0.0);

    double pOpponent = (currentPlayer == 0) ? p1 : p0;

    if (currentPlayer == 0) {
        for (int i = 0; i < game.actionCount; ++i) {
            actionsEV[i] = - vanillaCFR(game, state, history * 10 + game.actions[i], p0 * strategy[i], p1, pChance);
            currentEV += strategy[i] * actionsEV[i];
        }
    } else {
        for (int i = 0; i < game.actionCount; ++i) {
            actionsEV[i] = - vanillaCFR(game, state, history * 10 + game.actions[i], p0, p1 * strategy[i], pChance);
            currentEV += strategy[i] * actionsEV[i];
        }
    }

    std::vector<double> actionCFR(game.actionCount, 0.0);

    for (int i = 0; i < game.actionCount; ++i) {
        actionCFR[i] = pChance * pOpponent * (actionsEV[i] - currentEV);
    }

    infoSet.updateCFRSum(actionCFR);
    infoSet.updateStrategySum(strategy);

    return currentEV;
}