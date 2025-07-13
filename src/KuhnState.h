#ifndef KUHNSTATE_H
#define KUHNSTATE_H

#include "Action.h"
#include "GameState.h"
#include <iostream>
#include <vector>

class KuhnState : public GameState {
  public:
	static const int JACK = 0;
	static const int QUEEN = 1;
	static const int KING = 2;

	KuhnState();
	~KuhnState() override;

	bool isTerminal() const override;
	bool isChance() const override;
	double getUtility(int player) const override;

	std::vector<Action> getLegalActions() const override;
	std::unordered_map<Chance, double> getChance() const override;
	void applyAction(Action action) override;
	void applyChance(Chance chance) override;

	long getKey() const override;
	int getActionIndex(Action action) const override;
	std::unique_ptr<GameState> clone() const override;

  private:
	// Kuhn Poker specific state variables
	std::vector<int> deck;
	std::vector<Chance> chances;
	std::vector<bool> playerIsWinner;
	std::vector<bool> playerFolded;
	std::vector<double> playerAnte;
	std::vector<int> playerCards;
	bool isTerminalState;
	bool isChanceState;
};

#endif // KUHNSTATE_H