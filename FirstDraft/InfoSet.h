#pragma once

#include <vector>
#include <iostream>
#include <string>

struct InfoSet {
public:
    int actionCount;
    int history;
    int card;

    std::vector<double> regretSum;
    std::vector<double> strategySum;

    InfoSet() {}

    InfoSet(int actionCount, int history, int card) {
        this->actionCount = actionCount;
        this->history = history;
        this->card = card;
        
        regretSum = std::vector<double>(actionCount, 0.0);
        strategySum = std::vector<double>(actionCount, 0.0);
    }

    std::vector<double> getStrategy() {
        double normalizingSum = 0;

        std::vector<double> strategy(actionCount, 1.0 / actionCount);

        for (int i = 0; i < actionCount; ++i) {
            normalizingSum += std::max(regretSum[i], 0.0);
        }

        if (normalizingSum == 0) {
            return strategy;
        }

        for (int i = 0; i < actionCount; ++i) {
            strategy[i] = std::max(regretSum[i], 0.0) / normalizingSum;
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

    void updateStrategySum(std::vector<double> &strategy) {
        for (int i = 0; i < actionCount; ++i) {
            strategySum[i] += strategy[i];
        }
    }

    std::string toString() {
        std::string s = "";
        double sum = 0.0;
        for (int i = 0; i < actionCount; ++i) {
            sum += strategySum[i];
        }

        if (history == 33) {
			s += std::to_string(history) + " " + std::to_string(card) + "   ";
		} else {
			s += std::to_string(history) + " " + std::to_string(card) + " ";
		}
		if (sum == 0) {
			s += std::to_string((double)1 / 2) + " " + std::to_string((double)1 / 2);
		} else {
			s += std::to_string((double)strategySum[0] / sum) + " " + std::to_string((double)strategySum[1] / sum);
		}

		return s;
    }
};