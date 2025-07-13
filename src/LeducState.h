#ifndef LEDUCSTATE_H
#define LEDUCSTATE_H

#include "GameState.h"

class LeducState : public GameState {
  public:
	static const int JACK = 0;
	static const int QUEEN = 1;
	static const int KING = 2;

	LeducState();
	virtual ~LeducState() = default;

	virtual bool isTerminal() const override;
	virtual bool isChance() const override;
	virtual double getUtility(int player) const override;
	virtual std::vector<Action> getLegalActions() const override;
	virtual std::unordered_map<Chance, double> getChance() const override;
	virtual void applyAction(Action action) override;
	virtual void applyChance(Chance chance) override;
	virtual long getKey() const override;
	virtual int getActionIndex(Action action) const override;
	virtual std::unique_ptr<GameState> clone() const override;

    private:
    std::vector<int> deck;
    std::vector<Chance> chances;
    std::vector<bool> playerIsWinner;
    std::vector<bool> playerFolded;
    std::vector<double> playerAnte;
    std::vector<int> playerCards;
    int roundCount;
    bool isTerminalState;
    bool isChanceState;
};

#endif // LEDUCSTATE_H
