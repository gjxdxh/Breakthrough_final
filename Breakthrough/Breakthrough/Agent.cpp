#include "stdafx.h"
#include "Agent.h"
#include "stdlib.h"
using namespace std;

#define BLACK 1
#define WHITE 2

Agent::Agent(int _side, int _type)
{
	side = _side;
	nodes = 0;
	num_moves = 0;
	total_time = 0;
	capture = 0;
	agentType = _type;
	searchDepth = 3;
}

Agent::Agent()
{

}

Agent::~Agent()
{
}
