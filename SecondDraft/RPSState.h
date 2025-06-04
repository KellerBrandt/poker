#include "GameEnums.h"
#include "GameState.h"
#include <vector>
#include <utility>

class RPSState : public GameState {
public:
	Player currentPlayer;
	bool isTerminalNode;

	std::vector<Action> actions;

    RPSState();

    Player getCurrentPlayer() const override;

    bool isTerminal() const override;
    
    bool isChance() const override;

	double getUtility() const override;

	std::vector<Action> legalActions() const override;

	std::unique_ptr<GameState> clone() const override;

	void applyAction(Action action) override;

	void applyChance(Chance chance) override;

	std::vector<std::pair<Chance, double>> chanceOutcomes() const override;

	int getKey() const override;

	void print() const override;
	
};