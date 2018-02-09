#pragma once
#include "Agent.h"
#include "State.h"
class AgentAlphaBeta :
	public Agent
{
public:
	AgentAlphaBeta(int _side, int _type);
	~AgentAlphaBeta();
	double evaluate(State s);
	void doMove(State & current);
private:
	State alphabeta(State & s, int depth, double alpha, double beta, bool maxPlayer);
	State maxValue(State & s, double alpha, double beta, int depth);
	State minValue(State & s, double alpha, double beta, int depth);
};
