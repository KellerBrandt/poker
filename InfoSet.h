#pragma once

#include <vector>
#include <iostream>

struct InfoSet {
public:
    int actionCount;

    std::vector<double> regretSum;
    std::vector<double> strategySum;

    InfoSet(int actionCount) {
        this->actionCount = actionCount;
        regretSum = std::vector<double>(actionCount, 0.0);
        strategySum = std::vector<double>(actionCount, 0.0);
    }

    std::vector<double> getStrategy() {
        double normalizingSum = 0;

        std::vector<double> strategy(actionCount, 1.0 / actionCount);

        for (int i = 0; i < actionCount; ++i) {
            normalizingSum += std::max(strategy[i], 0.0);
        }

        if (normalizingSum == 0) {
            return strategy;
        }

        for (int i = 0; i < actionCount; ++i) {
            strategy[i] = regretSum[i] / normalizingSum;
        }

        return strategy;
    }

    std::vector<double> getAverageStrategy() {
        double normalizingSum = 0;
        
        for (int i = 0; i < actionCount; ++i) {
            normalizingSum += strategySum[i];
        }

        std::vector<double> averageStrategy;

        if (normalizingSum == 0) {
            for (int i = 0; i < actionCount; ++i) {
                averageStrategy.push_back(1.0 / actionCount);
            }

            return averageStrategy;
        }

        for (int i = 0; i < actionCount; ++i) {
            averageStrategy.push_back(strategySum[i] / normalizingSum);
        }

        return averageStrategy;
    }

    void updateCFRSum(std::vector<double> &actionCFR) {
        for (int i = 0; i < actionCount; ++i) {
            regretSum[i] += actionCFR[i];
        }
    }
};