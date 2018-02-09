#include "stdafx.h"
#include "AgentMinimax.h"
#include "Evaluate.h"
#include <algorithm>
#include <queue>
#include <climits>
#include <ctime>
#include <iostream>
using namespace std;

AgentMinimax::AgentMinimax(int _side, int _type)
{
	side = _side;
	nodes = 0;
	num_moves = 0;
	total_time = 0;
	capture = 0;
	agentType = _type;
	searchDepth = 3;
}


AgentMinimax::~AgentMinimax()
{
}

// Do a single move according to current board
void AgentMinimax::doMove(State & current)
{
	num_moves++;
	State temp = current;
	time_t beginTime, endTime;
	time(&beginTime);
	minimax(current, searchDepth, true);
	time(&endTime);
	total_time += difftime(endTime, beginTime);
	if (temp.getOpponentPieces(side) > current.getOpponentPieces(side))
	{
		capture++;
	}
}

double AgentMinimax::minimax(State & s, int depth, bool maxPlayer)
{
	nodes++;
	if (depth == 0 || s.isEnd())
		return evaluate(s);
	int moveSide;
	if (maxPlayer)
		moveSide = side;
	else
		(side == 1) ? (moveSide = 2) : (moveSide = 1);
	vector<State> moves = s.getPossibleStates(moveSide);	// flip side in each recursion
	if (maxPlayer)
	{
		double maxVal = -FLT_MAX;
		for (auto move : moves)
		{
			double minimaxVal = minimax(move, depth - 1, false);
			if (minimaxVal > maxVal)
			{
				maxVal = minimaxVal;
				if (depth == searchDepth)
					s = move;
			}
		}
		return maxVal;
	}
	else
	{
		double minVal = FLT_MAX;
		for (auto move : moves)
		{
			double minimaxVal = minimax(move, depth - 1, true);
			if (minimaxVal < minVal)
			{
				minVal = minimaxVal;
				if (depth == searchDepth)
					s = move;
			}
		}
		return minVal;
	}
}

// Evaluation function
// Choose different function according to the type of the agent
double AgentMinimax::evaluate(State s)
{
	if (agentType == 0)
		return Evaluate::defensive(s, side);
	else if (agentType == 1)
		return Evaluate::offensive(s, side);
	else
		return 0;
}
