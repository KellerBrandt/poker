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
	const double epsilon = 1e-16;

	CFR();
	~CFR();

	std::vector<double> vanillaCFR(std::unique_ptr<GameState> &state, int iterations);
	std::vector<double> CFRPlus(std::unique_ptr<GameState> &state, int iterations);

	double vanillaCFR(std::unique_ptr<GameState> &state, int traverser, double pi, double po);
	double CFRPlus(std::unique_ptr<GameState> &state, int traverser, double pi, double po);

	std::unordered_map<long, InfoSet> infoSets;
};

#endif // CFR_H