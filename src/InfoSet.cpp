#include "InfoSet.h"

double epsilon = 0.0;

InfoSet::InfoSet(std::vector<Action> actions, int player) : player(player) {
	for (const auto &action : actions) {
		regretSum[action] = 0.0;
		strategySum[action] = 0.0;
	}
}

InfoSet::~InfoSet() {
	// Destructor - vectors will be automatically cleaned up
}

std::unordered_map<Action, double> InfoSet::getStrategy() const {
    std::unordered_map<Action, double> strategy;
    double positiveRegretSum = 0.0;

    for (const auto &pair : regretSum) {
        positiveRegretSum += std::max(0.0, pair.second);
    }

    if (positiveRegretSum > 0.0) {
        for (const auto &pair : regretSum) {
            strategy[pair.first] = std::max(0.0, pair.second) / positiveRegretSum;
        }
    } else {
        // Play uniformly, but do NOT accumulate this strategy into the strategySum!
        double uniform = 1.0 / static_cast<double>(regretSum.size());
        for (const auto &pair : regretSum) {
            strategy[pair.first] = uniform;
        }
    }

    return strategy;
}


std::unordered_map<Action, double> InfoSet::getAverageStrategy() const {
	std::unordered_map<Action, double> averageStrategy;
	double totalStrategySum = 0.0;

	for (const auto &pair : strategySum) {
		totalStrategySum += pair.second;
		averageStrategy[pair.first] = 0.0;
	}

	if (totalStrategySum <= 0.0) {
		return averageStrategy;
	}

	for (const auto &pair : strategySum) {
		averageStrategy[pair.first] = pair.second / totalStrategySum;
	}

	return averageStrategy;
}