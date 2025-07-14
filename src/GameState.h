#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Action.h"
#include "Chance.h"
#include <memory>
#include <vector>

class GameState {
  public:
	GameState(int actionCount, int currentPlayer) : actionCount(actionCount), currentPlayer(currentPlayer) {}
	virtual ~GameState() = default;
	virtual bool isTerminal() const = 0;
	virtual bool isChance() const = 0;
	virtual double getUtility(int player) const = 0;
	virtual std::vector<Action> getLegalActions() const = 0;
	virtual std::unordered_map<Chance, double> getChance() const = 0;
	virtual void applyAction(Action action) = 0;
	virtual void revertAction(Action action) = 0;
	virtual void applyChance(Chance chance) = 0;
	virtual void revertChance(Chance chance) = 0;
	virtual long getKey() const = 0;
	virtual int getActionIndex(Action action) const = 0;
	virtual std::unique_ptr<GameState> clone() const = 0;
	int currentPlayer;
	int actionCount;
};

#endif // GAMESTATE_H