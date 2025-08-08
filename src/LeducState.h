#ifndef LEDUCSTATE_H
#define LEDUCSTATE_H

#include "Action.h"
#include "Chance.h"
#include "GameState.h"
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>

class LeducState : public GameState {
  public:
	static const int JACK = 0;
	static const int QUEEN = 1;
	static const int KING = 2;

	LeducState();
	~LeducState() override = default;

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
		int prevRound;
		int prevRaises;
		int prevToCall;
		bool didAdvanceRound;
		bool didTerminate;
		int prevPublicCard;
		double anteDeltaP0;
		double anteDeltaP1;
		bool prevFoldP0;
		bool prevFoldP1;
	};

	// betting round: 0 (preflop), 1 (postflop)
	int round;
	int raisesThisRound;
	int toCall;		// amount current player must call in this round (0, 2, or 4 depending on round)
	int publicCard; // -1 if not dealt
	bool isTerminalState;
	bool isChanceState;

	std::vector<int> deckCounts;  // counts per rank: size 3, starts [2,2,2]
	std::vector<Chance> chances;  // chance history (cards dealt)
	std::vector<int> playerCards; // private cards for P0,P1 (ranks 0..2)

	std::vector<Action> actionsRound0; // action history for round 0
	std::vector<Action> actionsRound1; // action history for round 1
	std::vector<ActionRecord> history; // for clean reverts

	std::vector<double> playerAnte; // contributions (start with 1 each)
	std::vector<bool> playerFolded; // who folded (if any)

	inline int betSize() const { return round == 0 ? 2 : 4; }

	// showdown helpers
	int winner() const; // -1 = tie, 0 or 1 = winner index
};

#endif // LEDUCSTATE_H
