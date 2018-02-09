#include "stdafx.h"
#include "State.h"
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#include <iostream>
using namespace std;

State::State(int _mode)
{
	gameMode = _mode;
	if (_mode == 0 || _mode == 1)
	{
		blackPieces = 16;
		whitePieces = 16;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 2; j++)
				board[i][j] = WHITE;
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 2; j < 6; j++)
				board[i][j] = EMPTY;
		}
		for (int i = 0; i < 8; i++)
		{
			for (int j = 6; j < 8; j++)
				board[i][j] = BLACK;
		}
		width = 8;
		height = 8;
	}
	else if (_mode == 2)
	{
		blackPieces = 20;
		whitePieces = 20;
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 2; j++)
				board[i][j] = WHITE;
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 2; j < 3; j++)
				board[i][j] = EMPTY;
		}
		for (int i = 0; i < 10; i++)
		{
			for (int j = 3; j < 5; j++)
				board[i][j] = BLACK;
		}
		width = 10;
		height = 5;
	}
	v = 0;
}


State::~State()
{
}

State::State(const State & other)
{
	gameMode = other.gameMode;
	width = other.width;
	height = other.height;
	blackPieces = other.blackPieces;
	whitePieces = other.whitePieces;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			board[i][j] = other.board[i][j];
		}
	}
	v = other.v;
}

// Don't know why this is not working...
// Using the set() function below to change modify this object
// @BUG
State& State::operator=(const State & other)
{
	gameMode = other.gameMode;
	width = other.width;
	height = other.height;
	blackPieces = other.blackPieces;
	whitePieces = other.whitePieces;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			board[i][j] = other.board[i][j];
		}
	}
	v = other.v;
	return *this;
}

// Get possible moves according to player's side and current state
// 1 for the black, 2 for the white
vector<State> State::getPossibleStates(int side)
{
	vector<State> possibleStates;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (board[i][j] == side)
			{
				// Black goes down
				if (side == BLACK)
				{
					// Goes down left
					if (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1] != BLACK)
					{
						State nextState = *this;
						nextState.board[i - 1][j - 1] = BLACK;
						nextState.board[i][j] = EMPTY;
						if (board[i - 1][j - 1] == WHITE)	// Eat white
							nextState.whitePieces--;
						possibleStates.push_back(nextState);
					}
					// Goes down
					if (j - 1 >= 0 && board[i][j - 1] == EMPTY)
					{
						State nextState = *this;
						nextState.board[i][j - 1] = BLACK;
						nextState.board[i][j] = EMPTY;
						possibleStates.push_back(nextState);
					}
					// Goes down right
					if (i + 1 < width && j - 1 >= 0 && board[i + 1][j - 1] != BLACK)
					{
						State nextState = *this;
						nextState.board[i + 1][j - 1] = BLACK;
						nextState.board[i][j] = EMPTY;
						if (board[i + 1][j - 1] == WHITE)	// Eat white
							nextState.whitePieces--;
						possibleStates.push_back(nextState);
					}
				}
				// White goes up
				else
				{
					
					// Goes up left
					if (i - 1 >= 0 && j + 1 >= 0 && board[i - 1][j + 1] != WHITE)
					{
						State nextState = *this;
						nextState.board[i - 1][j + 1] = WHITE;
						nextState.board[i][j] = EMPTY;
						if (board[i - 1][j + 1] == BLACK)	// Eat black
							nextState.blackPieces--;
						possibleStates.push_back(nextState);
					}
					// Goes up
					if (j + 1 < height && board[i][j + 1] == EMPTY)
					{
						State nextState = *this;
						nextState.board[i][j + 1] = WHITE;
						nextState.board[i][j] = EMPTY;
						possibleStates.push_back(nextState);
					}
					// Goes up right
					if (i + 1 < width && j + 1 < height && board[i + 1][j + 1] != WHITE)
					{
						State nextState = *this;
						nextState.board[i + 1][j + 1] = WHITE;
						nextState.board[i][j] = EMPTY;
						if (board[i + 1][j + 1] == BLACK)	// Eat black
							nextState.blackPieces--;
						possibleStates.push_back(nextState);
					}
				}
			}
		}
	}
	return possibleStates;
}

int State::getOpponentPieces(int side)
{
	if (side == BLACK)
		return whitePieces;
	else
		return blackPieces;
}

// Check if the game is end.
bool State::isEnd()
{
	// One worker win rule
	if (gameMode == 0 || gameMode == 2)
	{
		if (whitePieces == 0 || blackPieces == 0)
			return true;
		for (int i = 0; i < width; i++)
		{
			if (board[i][0] == BLACK || board[i][height - 1] == WHITE)
				return true;
		}
		return false;
	}
	// Three workers win rule
	else
	{
		if (whitePieces < 3 || blackPieces < 3)
			return true;
		int win_black = 0;
		int win_white = 0;
		for (int i = 0; i < width; i++)
		{
			if (board[i][0] == BLACK)
				win_black++;
			if (board[i][height - 1] == WHITE)
				win_white++;
		}
		if (win_black == 3 || win_white == 3)
			return true;
		return false;
	}
}

// Print the current board
// 'B' for black pieces, 'W' for white pieces, 'X' for empty
void State::print_state()
{
	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			if (board[j][i] == BLACK)
				cout << "B";
			else if (board[j][i] == WHITE)
				cout << "W";
			else
				cout << "X";
		}
		cout << endl;
	}
}

void State::set(State & other)
{
	gameMode = other.gameMode;
	width = other.width;
	height = other.height;
	blackPieces = other.blackPieces;
	whitePieces = other.whitePieces;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			board[i][j] = other.board[i][j];
		}
	}
	v = other.v;
}

int State::getPieces(int _side)
{
	if (_side == WHITE)
		return whitePieces;
	else
		return blackPieces;
}