#ifndef RPSSTATE_H
#define RPSSTATE_H

#include "Action.h"
#include "Chance.h"
#include "GameState.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class RPSState : public GameState {
  public:
	RPSState();
	~RPSState() override;

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
	std::vector<Action> actions;
};

#endif // RPSSTATE_H
