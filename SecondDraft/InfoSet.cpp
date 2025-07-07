#include "InfoSet.h"

InfoSet::InfoSet() {}

InfoSet::InfoSet(int numActions) : numActions(numActions), regretSum(numActions, 0.0), strategySum(numActions, 0.0) {}

std::vector<double> InfoSet::getStrategy() {
	double sum = 0.0;

	for (auto &r : regretSum) {
		sum += std::max(0.0, r);
	}

	std::vector<double> strategy(numActions, 1.0 / static_cast<double>(numActions));

	if (sum > 0.0) {
		for (int i = 0; i < numActions; i++) {
			strategy[i] = std::max(0.0, regretSum[i]) / sum;
		}
	}
	
	return strategy;
}

std::vector<double> InfoSet::getAverageStrategy() const {
	double sum = 0.0;

	for (auto &s : strategySum) {
		sum += s;
	}

	std::vector<double> averageStrategy(numActions, 1.0 / static_cast<double>(numActions));

	if (sum > 0) {
		for (int i = 0; i < numActions; i++) {
			averageStrategy[i] = strategySum[i] / sum;
		}
	}

	return averageStrategy;
}

void InfoSet::updateStrategySum(const std::vector<double> &strategy) {
	for (int i = 0; i < numActions; i++) {
		strategySum[i] += strategy[i];
	}
}

void InfoSet::updateRegretSum(const std::vector<double> &regret) {
	for (int i = 0; i < numActions; i++) {
		regretSum[i] += regret[i];
	}
}