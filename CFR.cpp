#include "CFR.h"

void CFR::runVanillaCFR(Game &game) {}

double CFR::vanillaCFR(Game &game, int history, double p0, double p1, double &pChance) {
    int currentPlayer = game.getCurrentPlayer(history);

    if (game.isTerminal(history)) {
        return game.getUtility(history, currentPlayer);
    }
    
    int key = game.getKey(currentPlayer, history);
    
    if (game.infoSets.find(key) == game.infoSets.end()) {
        game.infoSets[key] = InfoSet(game.actionCount);
    }

    InfoSet &infoSet = game.infoSets[key];

    std::vector<double> strategy = infoSet.getStrategy();

    double currentEV = 0.0; //counterfactual value of the node given the current strategy; i.e. expected value of the node
    std::vector<double> actionsEV(game.actionCount, 0.0);

    double pOpponent = (currentPlayer == 0) ? p1 : p0;

    if (currentPlayer == 0) {
        for (int i = 0; i < game.actionCount; ++i) {
            actionsEV[i] = vanillaCFR(game, history * 10 + i, p0 * strategy[i], p1, pChance);
            currentEV += strategy[i] * actionsEV[i];
        }
    } else {
        for (int i = 0; i < game.actionCount; ++i) {
            actionsEV[i] = vanillaCFR(game, history * 10 + i, p0, p1 * strategy[i], pChance);
            currentEV += strategy[i] * actionsEV[i];
        }
    }

    std::vector<double> actionCFR(game.actionCount, 0.0);

    for (int i = 0; i < game.actionCount; ++i) {
        actionCFR[i] = pChance * pOpponent * (actionsEV[i] - currentEV);
    }

    infoSet.updateCFRSum(actionCFR);

    return - currentEV;
}