#pragma once
#include "State.h"
class Evaluate
{
public:
	Evaluate();
	~Evaluate();
	static double defensive(State s, int side);
	static double offensive(State s, int side);
	static double advDefensive(State s, int side);
	static double advOffensive(State s, int side);
};

