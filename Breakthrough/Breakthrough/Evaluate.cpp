#include "stdafx.h"
#include "Evaluate.h"

#define EMPTY 0
#define BLACK 1
#define WHITE 2

Evaluate::Evaluate()
{
}


Evaluate::~Evaluate()
{
}

double Evaluate::defensive(State s, int side)
{
	if (side == BLACK)
		return 2 * s.blackPieces + ((double)rand() / (RAND_MAX));
	else
		return 2 * s.whitePieces + ((double)rand() / (RAND_MAX));
}

double Evaluate::offensive(State s, int side)
{
	if (side == BLACK)
		return 2 * (30 - s.whitePieces) + ((double)rand() / (RAND_MAX));
	else
		return 2 * (30 - s.blackPieces) + ((double)rand() / (RAND_MAX));
}

// A helper function that checks if current position is being protected
bool isProtected(State s, int i, int j, int side)
{
	if (side == BLACK)
	{
		if (i - 1 >= 0 && j + 1 < s.height && s.board[i - 1][j + 1] == BLACK)	// up left protection
			return true;
		else if (i + 1 < s.width && j + 1 < s.height && s.board[i + 1][j + 1] == BLACK)	// up right protection
			return true;
	}
	else
	{
		if (i - 1 >= 0 && j - 1 >= 0 && s.board[i - 1][j - 1] == WHITE)
			return true;
		else if (i + 1 < s.width && j - 1 < s.height && s.board[i + 1][j - 1] == WHITE)
			return true;
	}
	return false;
}

// A helper function that checks if current position is being attacked
bool isAttacked(State s, int i, int j, int side)
{
	if (side == BLACK)
	{
		if (i - 1 >= 0 && j - 1 < s.height && s.board[i - 1][j - 1] == WHITE)	
			return true;
		else if (i + 1 < s.width && j - 1 < s.height && s.board[i + 1][j - 1] == WHITE)	
			return true;
	}
	else
	{
		if (i - 1 >= 0 && j + 1 >= 0 && s.board[i - 1][j + 1] == BLACK)
			return true;
		else if (i + 1 < s.width && j + 1 < s.height && s.board[i + 1][j + 1] == BLACK)
			return true;
	}
	return false;
}

double Evaluate::advDefensive(State s, int side)
{
	// Some random noise to break tie
	double noise = (double)rand() / RAND_MAX;
	double h = 0;
	if (side == BLACK)
	{
		// Winning value: if all enemies are killed
		if (s.whitePieces == 0)
			return 10000;
		for (int i = 0; i < s.width; i++)
		{
			// Column Hole value: avoid empty column and try to block enemy
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == BLACK)
				{
					h += 1;
					break;
				}
			}
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == BLACK)
				{
					// Surviving value: each surviving piece add points
					h += 1;
					// Winning value: reach the bottom
					if (j == 0)
						h += 1000;
					// Danger value: stay away from enemies' territory
					h += (s.height / 2.0);
					// Horizontal connection value: form "wall" to defend
					if (i - 1 > 0 && s.board[i - 1][j] == BLACK)
						h += 0.8;
					if (i + 1 < s.width && s.board[i + 1][j] == BLACK)
						h += 0.8;
					// Protected value: if piece is being protected
					if (isProtected(s, i, j, BLACK))
						h += 2;
					else
					{
						if (isAttacked(s, i, j, BLACK))
							h -= 2;
					}
					// Control value: if piece control some empty area
					if (i - 1 >= 0 && j - 1 >= 0 && s.board[i - 1][j - 1] == EMPTY)	// down left control
						h += 1;
					if (i + 1 < s.width && j - 1 >= 0 && s.board[i + 1][j - 1] == EMPTY)	// down right control
						h += 1;
				}
				// Attack value: encourage attack slightly
				if (s.board[i][j] == WHITE)
				{
					// Enemy approaching value: kill or block enemies that are too close to victory
					h -= (j / 1.5);
				}
			}
		}
	}
	else
	{
		// Winning value: if all enemies are destroyed
		if (s.blackPieces == 0)
			return 10000;
		for (int i = 0; i < s.width; i++)
		{
			// Column Hole value: avoid empty column and try to block enemy
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == WHITE)
				{
					h += 1;
					break;
				}
			}
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == WHITE)
				{
					// Surviving value: each surviving piece add points
					h += 1;
					// Winning value: reach the bottom
					if (j == 0)
						h += 1000;
					// Danger value: stay away from enemies' territory
					h += ((s.height - j) / 2.0);
					// Horizontal connection value: form "wall" to defend
					if (i - 1 > 0 && s.board[i - 1][j] == WHITE)
						h += 0.8;
					if (i + 1 < s.width && s.board[i + 1][j] == WHITE)
						h += 0.8;
					// Protected value: if piece is being protected
					if (isProtected(s, i, j, WHITE))
						h += 2;
					else
					{
						// If is not protected, subtract a danger value
						if (isAttacked(s, i, j, WHITE))
							h -= 2;
					}
					// Control value: if piece control some empty area
					if (i - 1 >= 0 && j + 1 < s.width && s.board[i - 1][j + 1] == EMPTY)	// down left control
						h += 1;
					if (i + 1 < s.width && j + 1 < s.height && s.board[i + 1][j + 1] == EMPTY)	// down right control
						h += 1;
				}
				// Attack value: encourage attack slightly
				if (s.board[i][j] == BLACK)
					h -= ((s.height - j - 1) / 1.5);
			}
		}
	}
	return h + noise;
}

double Evaluate::advOffensive(State s, int side)
{
	double noise = (double)rand() / RAND_MAX;
	double h = 0;
	if (side == BLACK)
	{
		// Winning value
		if (s.whitePieces == 0)
			return 10000;
		// Some values for global heuristic calculation
		int front = 0;
		for (int i = 0; i < s.width; i++)
		{
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == BLACK)
				{
					// Winning value: reach the bottom
					if (j == 0)
						h += 1000;
					// Almost win value: closer to bottom
					h += (double)((s.height - j) / 2.0);
					if (s.height - j > front)
						front = s.height - j;
					// Column value: go to the columns with less opponent pieces
					double opponent = 0;
					for (int k = 0; k < s.height; k++)
					{
						if (s.board[i][k] == WHITE)
							opponent++;
					}
					h += ((s.height - opponent) / 2.0);
				}
				// Attack value: try to attack more enemies
				if (s.board[i][j] == WHITE)
					h -= 3;
			}
		}
		h += (double)front;
	}
	else
	{
		// Winning value
		if (s.blackPieces == 0)
			return 10000;
		// Some values for global heuristic calculation
		int front = 0;
		for (int i = 0; i < s.width; i++)
		{
			for (int j = 0; j < s.height; j++)
			{
				if (s.board[i][j] == WHITE)
				{
					// Winning value: reach the bottom
					if (j == s.height - 1)
						h += 1000;
					// Almost win value: closer to bottom
					h += (double)(s.height / 2.0);
					if (s.height > front)
						front = s.height;
					// Column value: go to the columns with less opponent pieces
					double opponent = 0;
					for (int k = 0; k < s.height; k++)
					{
						if (s.board[i][k] == BLACK)
							opponent++;
					}
					h += ((s.height - opponent) / 2.0);
				}
				// Attack value: try to attack more enemies
				if (s.board[i][j] == BLACK)
					h -= 3;
			}
		}
		h += (double)front;
	}
	return h + noise;
}