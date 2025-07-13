#include "CFR.h"
#include "KuhnState.h"
#include "RPSState.h"
#include <iostream>

int main() {
	std::unique_ptr<GameState> root = std::make_unique<KuhnState>();
	CFR cfr;
	std::vector<double> temp = cfr.vanillaCFR(root, 100000);
	std::cout << "0: " << temp[0] << " | 1: " << temp[1] << std::endl;

	std::cout << "\nAverage strategies for all InfoSets:" << std::endl;
	// Compute max length of 'InfoSet key: <key>,' for alignment
	int max_key_len = 0;
	for (const auto &e : cfr.infoSets) {
		int len = std::string("InfoSet key: ").size() + std::to_string(e.first).size() + 1; // 1 for ','
		if (len > max_key_len) max_key_len = len;
	}
	const std::string label = " avg strategy: ";
	const int tuple_start_col = max_key_len + label.size();

	for (const auto &entry : cfr.infoSets) {
		std::string key_str = "InfoSet key: " + std::to_string(entry.first) + ",";
		std::cout << key_str;
		std::cout << std::string(max_key_len - key_str.size(), ' ');
		std::cout << label;

		int current_col = max_key_len + label.size();
		if (current_col < tuple_start_col) {
			std::cout << std::string(tuple_start_col - current_col, ' ');
		}

		const auto &avg = entry.second.getAverageStrategy();
		std::vector<std::string> tuple_strs;
		for (const auto &ap : avg) {
			tuple_strs.push_back("(" + std::to_string(static_cast<int>(ap.first.type)) + ": " + std::to_string(ap.second) + ")");
		}
		int first_tuple_len = tuple_strs.empty() ? 0 : tuple_strs[0].size();
		int second_tuple_col = tuple_start_col + first_tuple_len + 3; // 3 spaces between
		for (size_t i = 0; i < tuple_strs.size(); ++i) {
			if (i > 0) {
				std::cout << std::string(second_tuple_col - (tuple_start_col + first_tuple_len), ' ');
			}
			std::cout << tuple_strs[i];
		}
		std::cout << std::endl;

	}

	return 0;
}