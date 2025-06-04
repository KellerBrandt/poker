#pragma once

//General
enum Player {
    PLAYER0 = 0,
    PLAYER1 = 1,
    CHANCE = 2
};

/*
//RPS
enum Action {
	NULL_ACTION = -1,
	ROCK = 0,
	PAPER = 1,
	SCISSORS = 2,
	ACTION_COUNT = 3
};

enum Chance {
	NULL_CHANCE = -1
};
*/


//KuhnPoker
enum Action {
    NULL_ACTION = -1,
    CHECK = 0,
    BET = 1,
    ACTION_COUNT = 2
};

enum Card {
    NULL_CARD = -1,
    JACK = 0,
    QUEEN = 1,
    KING = 2
}; 

enum Chance {
    NULL_CHANCE = -1,
    DEAL_JACK = 0,
    DEAL_QUEEN = 1,
    DEAL_KING = 2
};


/*
//Leduc
enum Action {
    FOLD = 0,
    CALL = 1,
    RAISE = 2,
    ACTION_COUNT = 3
};

enum Card {
    JACK = 0,
    QUEEN = 1,
    KING = 2
}; 

enum Chance {
    DEAL_JACK = 0,
    DEAL_QUEEN = 1,
    DEAL_KING = 2
};
*/