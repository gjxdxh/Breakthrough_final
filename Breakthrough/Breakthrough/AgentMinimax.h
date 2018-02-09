#pragma once
#include "Agent.h"
class AgentMinimax :
	public Agent
{
public:
	AgentMinimax(int _side, int _type);
	~AgentMinimax();
	void doMove(State & current);
	double evaluate(State s);
private:
	double minimax(State & s, int depth, bool maxPlayer);
};

