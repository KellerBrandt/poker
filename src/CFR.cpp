#include "CFR.h"
#include <iostream>

CFR::CFR() {
	// Constructor - initialize CFR algorithm
}

CFR::~CFR() {
	// Destructor - cleanup
}

std::vector<double> CFR::vanillaCFR(std::unique_ptr<GameState> &state, int iterations) {
	double EV0 = 0;
	double EV1 = 0;

	for (int i = 0; i < iterations; ++i) {
		EV0 += vanillaCFR(state, 0, 1.0, 1.0);
		EV1 += vanillaCFR(state, 1, 1.0, 1.0);
	}

	EV0 /= iterations;
	EV1 /= iterations;

	return {EV0, EV1};
}

double CFR::vanillaCFR(std::unique_ptr<GameState> &state, int traverser, double pi, double po) {
	if (state->isTerminal()) {
		return state->getUtility(traverser);
	}

	double nodeUtility = 0.0;

	// chance node handling
	if (state->isChance()) {
		auto chances = state->getChance();

		for (const auto &chance : chances) {
			state->applyChance(chance.first);
			nodeUtility += chance.second * vanillaCFR(state, traverser, pi, po * chance.second);
			state->revertChance(chance.first);
		}

		return nodeUtility;
	}

	long key = state->getKey();
	auto legalActions = state->getLegalActions();
	int currentPlayer = state->currentPlayer;

	// If infoset does not exist for this key, create it
	if (infoSets.find(key) == infoSets.end()) {
		infoSets[key] = InfoSet(legalActions, currentPlayer);
	}
	InfoSet &infoSet = infoSets[key];

	auto strategy = infoSet.getStrategy();

	std::unordered_map<Action, double> actionUtilities;

	if (currentPlayer == traverser) {
		for (const auto &action : legalActions) {
			state->applyAction(action);
			actionUtilities[action] = vanillaCFR(state, traverser, pi * strategy[action], po);
			state->revertAction(action);

			nodeUtility += strategy[action] * actionUtilities[action];
		}
	} else if (currentPlayer == (1 - traverser)) {
		for (const auto &action : legalActions) {
			state->applyAction(action);
			actionUtilities[action] = vanillaCFR(state, traverser, pi, po * strategy[action]);
			state->revertAction(action);

			nodeUtility += strategy[action] * actionUtilities[action];
		}
	} else {
		std::cout << "ERROR IN CFR RECURSIVE CALL" << std::endl;
	}

	if (currentPlayer == traverser) {
		for (const auto &action : legalActions) {
			infoSet.strategySum[action] += pi * strategy[action];
			infoSet.regretSum[action] += po * (actionUtilities[action] - nodeUtility);
		}
	}

	return nodeUtility;
}