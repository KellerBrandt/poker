#pragma once

#include <vector>

class InfoSet {
public:
    int numActions;
    std::vector<double> regretSum;
    std::vector<double> strategySum;

	InfoSet();
    InfoSet(int numActions);

    std::vector<double> getStrategy();

    std::vector<double> getAverageStrategy() const;

    void updateStrategySum(const std::vector<double>& strategy);

    void updateRegretSum(const std::vector<double>& regret);
};