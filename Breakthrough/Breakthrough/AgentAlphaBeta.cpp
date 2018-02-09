#include "stdafx.h"
#include "AgentAlphaBeta.h"
#include "Evaluate.h"
#include <climits>
#include <algorithm>
#include <ctime>
#include <iostream>
using namespace std;

AgentAlphaBeta::AgentAlphaBeta(int _side, int _type)
{
	side = _side;
	nodes = 0;
	num_moves = 0;
	total_time = 0;
	capture = 0;
	agentType = _type;
	searchDepth = 4;
}


AgentAlphaBeta::~AgentAlphaBeta()
{
}

void AgentAlphaBeta::doMove(State & current)
{
	int piecesBefore = current.getOpponentPieces(side);
	num_moves++;
	time_t beginTime, endTime;
	time(&beginTime);
	current = alphabeta(current, searchDepth, -FLT_MAX, FLT_MAX, true);
	time(&endTime);
	total_time += difftime(endTime, beginTime);
	int piecesAfter = current.getOpponentPieces(side);
	if (piecesAfter > piecesBefore)
		capture++;
	//current.print_state();
}

State AgentAlphaBeta::alphabeta(State & s, int depth, double alpha, double beta, bool maxPlayer)
{
	return maxValue(s, -FLT_MAX, FLT_MAX, depth);
	// Below is another implementation of alphabeta pruning.
	// Statements below will not be runned and are for debugging purposes.
	// To use the implementation below, comment out the first line of this function.
	if (depth == 0 || s.isEnd())
	{
		s.v = evaluate(s);
		return s;
	}
	State action(s.gameMode);
	int moveSide;
	if (maxPlayer)
		moveSide = side;
	else
		(side == 1) ? (moveSide = 2) : (moveSide = 1);
	vector<State> moves = s.getPossibleStates(moveSide);	// flip side in each recursion
	if (maxPlayer)
	{
		action.v = -FLT_MAX;
		for (auto move : moves)
		{
			State temp = alphabeta(move, depth - 1, alpha, beta, false);
			if (temp.v > action.v)
				action = temp;
			if (action.v > beta)
				return action;
			alpha = max(alpha, action.v);
		}
		return action;
	}
	else
	{
		action.v = FLT_MAX;
		for (auto move : moves)
		{
			State temp = alphabeta(move, depth - 1, alpha, beta, true);
			if (temp.v < action.v)
				action = temp;
			if (action.v <= alpha)
				return action;
			beta = min(beta, action.v);
		}
		return action;
	}
}

State AgentAlphaBeta::maxValue(State & s, double alpha, double beta, int depth)
{
	nodes++;
	if (depth == 0 || s.isEnd()) 
	{
		s.v = evaluate(s);
		return s;
	}
	State retS(s.gameMode);
	retS.v = -FLT_MAX;
	int moveSide;
	if ((side + searchDepth - depth) % 2 == 0)
		moveSide = 2;
	else
		moveSide = 1;
	vector<State> possibleStates = s.getPossibleStates(moveSide);
	for (auto move : possibleStates)
	{
		State temp = minValue(move, alpha, beta, depth - 1);
		if (temp.v > retS.v)
		{
			retS = move;
			retS.v = temp.v;
		}
		if (retS.v >= beta)
			return retS;
		alpha = max(alpha, retS.v);
	}
	return retS;
}

State AgentAlphaBeta::minValue(State & s, double alpha, double beta, int depth)
{
	nodes++;
	if (depth == 0 || s.isEnd())
	{
		s.v = evaluate(s);
		return s;
	}
	State retS(s.gameMode);
	retS.v = FLT_MAX;
	int moveSide;
	if ((side + searchDepth - depth) % 2 == 0)
		moveSide = 2;
	else
		moveSide = 1;
	vector<State> possibleStates = s.getPossibleStates(moveSide);
	for (auto move : possibleStates)
	{
		State temp = maxValue(move, alpha, beta, depth - 1);
		if (temp.v < retS.v)
		{
			retS = move;
			retS.v = temp.v;
		}
		if (retS.v <= alpha)
			return retS;
		beta = min(beta, retS.v);
	}
	return retS;
}

double AgentAlphaBeta::evaluate(State s)
{
	if (agentType == 0)
		return Evaluate::defensive(s, side);
	else if (agentType == 1)
		return Evaluate::offensive(s, side);
	else if (agentType == 2)
		return Evaluate::advDefensive(s, side);
	else if (agentType == 3)
		return Evaluate::advOffensive(s, side);
	else
		return 0;
}
