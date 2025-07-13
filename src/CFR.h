#ifndef CFR_H
#define CFR_H

#include "GameState.h"
#include "InfoSet.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class CFR {
  public:
	CFR();
	~CFR();

	std::vector<double> vanillaCFR(std::unique_ptr<GameState> &state, int iterations);
	double dualCFR(std::unique_ptr<GameState> &state, int iterations);
	double vanillaCFR(std::unique_ptr<GameState> &state, int traverser, double pi, double po);
	double dualCFR(std::unique_ptr<GameState> &state, double pi, double po);

		std::unordered_map<long, InfoSet> infoSets;
	};

#endif // CFR_H