#include "CFR.h"
#include "GameState.h"
#include "KuhnState.h"
#include "LeducState.h"
#include "RPSState.h"
#include <chrono>
#include <iostream>

//g++ -std=c++17 -O2 -Wall -Wextra src/*.cpp -o poker

void runVanillaCFR(std::unique_ptr<GameState> &state, CFR &cfr, int runCount, int iterations) {
	std::cout << "Iterations: " << iterations << std::endl;
	std::chrono::duration<double> elapsed = std::chrono::duration<double>(0);
	std::vector<double> results = {0.0, 0.0};

	for (int i = 0; i < runCount; ++i) {
		auto start = std::chrono::high_resolution_clock::now();

		std::vector<double> temp = cfr.vanillaCFR(state, iterations);

		auto end = std::chrono::high_resolution_clock::now();

		results[0] += temp[0];
		results[1] += temp[1];

		elapsed += end - start;
	}

	elapsed /= runCount;

	results[0] /= runCount;
	results[1] /= runCount;

	std::cout << "0: " << results[0] << " | 1: " << results[1] << std::endl;
	std::cout << "Total InfoSet size: " << cfr.infoSets.size() << std::endl;
	std::cout << "CFR execution time: " << elapsed.count() << " seconds" << std::endl;
}

int main() {
	std::unique_ptr<GameState> root = std::make_unique<KuhnState>();
	CFR cfr;
	
	runVanillaCFR(root, cfr, 1, 100000);

	/*
	for (const auto &infoSet : cfr.infoSets) {
		auto averageStrategy = infoSet.second.getAverageStrategy();
		std::cout << infoSet.first << ": " << averageStrategy[Action(Action::Type::Rock)] << " " << averageStrategy[Action(Action::Type::Paper)] << " " << averageStrategy[Action(Action::Type::Scissors)] << std::endl;
	}
	*/

	return 0;
}