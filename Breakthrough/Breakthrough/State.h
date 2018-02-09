#pragma once
#include <vector>
using namespace std;
/** This class denotes the state of the board.
	For each square, 0 represents empty, 1 represents black, 2 represents white.
**/
class State
{
public:
	int board[10][10];	// State of the board
	int blackPieces;	// Num of black pieces
	int whitePieces;	// Num of white pieces
	double v;	// Store node value of the state. Used in AlphaBeta tree search.
	int gameMode;	// 0 for regular 8x8 game, 1 for 3 workers to base, 2 for long board
	int width, height;	// size of the board
	State& operator=(const State & other);
	State(int _mode);
	~State();
	State(const State & other);
	vector<State> getPossibleStates(int side);
	int getOpponentPieces(int side);
	int getPieces(int side);
	bool isEnd();
	void print_state();
	void set(State & other);
};

