#ifndef LEDUCSTATE_H
#define LEDUCSTATE_H

#include "Action.h"
#include "Chance.h"
#include "GameState.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class LeducState : public GameState {
  public:
	const int JACK = 0;
	const int QUEEN = 1;
	const int KING = 2;

	LeducState();
	~LeducState() = default;

	bool isTerminal() const override;
	bool isChance() const override;
	double getUtility(int player) const override;

	std::vector<Action> getLegalActions() const override;
	std::unordered_map<Chance, double> getChance() const override;
	void applyAction(Action action) override;
	void revertAction(Action action) override;
	void applyChance(Chance chance) override;
	void revertChance(Chance chance) override;

	long getKey() const override;
	int getActionIndex(Action action) const override;
	std::unique_ptr<GameState> clone() const override;

  private:
	int round;
	int raiseCount;
	int publicCard;
	int betAmount;
	bool isTerminalState;
	bool isChanceState;

	std::vector<Chance> chances;
	std::array<double, 2> playerAnte;
	std::array<bool, 2> playerFolded;
	std::array<bool, 2> playerWins;
	std::vector<int> playerCards;

	std::vector<std::vector<Action>> actions;

	std::vector<int> deck;
};

#endif // LEDUCSTATE_H