#pragma once
#pragma once
#include "State.h"
class Agent
{
public:
	Agent(int _side, int _type);
	Agent();
	~Agent();

	int side;
	int nodes;	// Total nodes expand
	int num_moves;	// Total number of moves
	double total_time;	// Total time spent
	int capture;	// Number of opponent workers captured
	int agentType;	// Type of the agent. Can be [0: defensive, 1: offensive, 2: advance defensive, 3: advance offensive]
	int searchDepth;

	virtual void doMove(State & doMove) = 0;
	virtual double evaluate(State s) = 0;
};

