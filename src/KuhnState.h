#ifndef KUHNSTATE_H
#define KUHNSTATE_H

#include "Action.h"
#include "Chance.h"
#include "GameState.h"
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

class KuhnState : public GameState {
  public:
	static const int JACK = 0;
	static const int QUEEN = 1;
	static const int KING = 2;

	KuhnState();
	~KuhnState() override = default;

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
	struct ActionRecord {
		Action action;
		int prevCurrentPlayer;
		bool prevIsTerminal;
		bool prevIsChance;
		int prevRaises;
		int prevToCall;
		double anteDeltaP0;
		double anteDeltaP1;
		bool prevFoldP0;
		bool prevFoldP1;
	};

	int raisesThisRound;
	int toCall; // 0 or 1
	bool isTerminalState;
	bool isChanceState;

	std::vector<int> deckCounts; // [1,1,1] for J,Q,K
	std::vector<Chance> chances;
	std::vector<int> playerCards; // size 2 when dealt

	std::vector<Action> actions; // single-round history
	std::vector<ActionRecord> history;

	std::vector<double> playerAnte; // start with 1 each
	std::vector<bool> playerFolded;

	inline int betSize() const { return 1; }
};

#endif // KUHNSTATE_H
