#ifndef INFOSET_H
#define INFOSET_H

#include "Action.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

// change infoset to take a set/array of actions, and change regretsum and strategy sum to unordered_maps
class InfoSet {
  public:
	InfoSet(std::vector<Action> = {}, int player = -1);
	~InfoSet();

	int player;
	double reachProb;
	std::unordered_map<Action, double> regretSum;
	std::unordered_map<Action, double> strategySum;

	// Helper functions
	std::unordered_map<Action, double> getStrategy() const;
	std::unordered_map<Action, double> getAverageStrategy() const;
};

#endif // INFOSET_H