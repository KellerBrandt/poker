#pragma once

#include <vector>

class InfoSet {
public:
    std::vector<double> regretSum;
    std::vector<double> strategySum;

    InfoSet(int numActions) : regretSum(numActions, 0.0), strategySum(numActions, 0.0) {}

    std::vector<double> getStrategy();

    std::vector<double> getAverageStrategy() const;

    void updateStrategySum(const std::vector<double>& strategy);

    void updateRegretSum(const std::vector<double>& regret);
};