#include "KuhnState.h"
//#include "RPSState.h"
#include "CFR.h"
#include <iostream>

int main() {
	std::unique_ptr<GameState> root = std::make_unique<KuhnState>();
	//std::unique_ptr<GameState> root = std::make_unique<RPSState>();
	CFR cfr;
	cfr.runVanillaCFR(root, 10000);
	return 0;
}