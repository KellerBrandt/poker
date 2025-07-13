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
		auto state0 = state->clone();
		auto state1 = state->clone();

		double temp0 = vanillaCFR(state0, 0, 1.0, 1.0);
		double temp1 = vanillaCFR(state1, 1, 1.0, 1.0);

		// std::cout << "temp0: " << temp0 << std::endl;
		// std::cout << "temp1: " << temp1 << std::endl;

		EV0 += temp0;
		EV1 += temp1;
	}

	EV0 /= iterations;
	EV1 /= iterations;

	return {EV0, EV1};
}

double CFR::dualCFR(std::unique_ptr<GameState> &state, int iterations) {
	double EV0 = 0;

	for (int i = 0; i < iterations; ++i) {
		EV0 += dualCFR(state, 1.0, 1.0);
	}

	EV0 /= iterations;

	return EV0;
}

/*
notes for speed:
long key = state->getKey()
actions = state->getLegalActions()
maybe remove double pc
make everything references
*/

double CFR::vanillaCFR(std::unique_ptr<GameState> &state, int traverser, double pi, double po) {
	if (state->isTerminal()) {
		return state->getUtility(traverser);
	}

	double nodeUtility = 0.0;

	// chance node handling
	if (state->isChance()) {
		auto chances = state->getChance();

		for (const auto &chance : chances) {
			auto nextState = state->clone();
			nextState->applyChance(chance.first);
			nodeUtility += chance.second * vanillaCFR(nextState, traverser, pi, po * chance.second);
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
			auto nextState = state->clone();
			nextState->applyAction(action);
			actionUtilities[action] = vanillaCFR(nextState, traverser, pi * strategy[action], po);
			nodeUtility += strategy[action] * actionUtilities[action];
		}
	} else if (currentPlayer == (1 - traverser)) {
		for (const auto &action : legalActions) {
			auto nextState = state->clone();
			nextState->applyAction(action);
			actionUtilities[action] = vanillaCFR(nextState, traverser, pi, po * strategy[action]);
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
		// std::cout << "KEY: " << key << "  ";
		// for (const auto &p : infoSet.regretSum)
		// 	std::cout << p.first.toString() << ":" << p.second << "  ";
		// std::cout << std::endl;
	}

	return nodeUtility;
}

double CFR::dualCFR(std::unique_ptr<GameState> &state, double pi, double po) {
	return -1.0;
}