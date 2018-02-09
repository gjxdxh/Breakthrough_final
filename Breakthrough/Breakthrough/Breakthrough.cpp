// Breakthrough.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AgentMinimax.h"
#include "AgentAlphaBeta.h"
#include "Agent.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <string>
using namespace std;
#define EMPTY 0
#define BLACK 1
#define WHITE 2

/*
	Agent type:
	0 - Naive Defensive
	1 - Naive Offensive
	2 - Advanced Defensive
	3 - Advanced Offensive
	Game Mode:
	0 - 3 cr regular 8x8 game
	1 - 3 workers win game
	2 - long board game
*/
int main()
{
	srand(time(0));
	AgentAlphaBeta a1(BLACK, 1);
	AgentAlphaBeta a2(WHITE, 2);
	int gameMode = 1;
	State init_state(gameMode);
	string winner = "";
	//init_state.print_state();
	while (1)
	{
		a1.doMove(init_state);
		//init_state.print_state();
		//cout << endl;
		if (init_state.isEnd())
		{
			
			winner = "BLACK";
			break;
		}
		a2.doMove(init_state);
		//init_state.print_state();
		//cout << endl;
		if (init_state.isEnd())
		{
			
			winner = "WHITE";
			break;
		}
	}
	init_state.print_state();
	cout << winner << " wins!" << endl;
	int pieces;
	if (gameMode == 0 || gameMode == 1)
		pieces = 16;
	else
		pieces = 20;
	//cout << "Black capture enemies: " << pieces - init_state.whitePieces << " " << "White capture enemies: " << pieces - init_state.blackPieces << endl;
	//cout << "Black moves: " << a1.num_moves << endl;
	//cout << "White moves: " << a2.num_moves << endl;
	//cout << "Black nodes expanded: " << a1.nodes << endl;
	//cout << "White nodes expanded: " << a2.nodes << endl;
	//cout << "Black average nodes expanded per step: " << (double)a1.nodes / (double)a1.num_moves << endl;
	//cout << "White average nodes expanded per step: " << (double)a2.nodes / (double)a2.num_moves << endl;
	//cout << "Black time spent on each move: " << a1.total_time / (double)a1.num_moves << endl;
	//cout << "White time spent on each move: " << a2.total_time / (double)a2.num_moves << endl;
	cout << pieces - init_state.whitePieces << "\t" << pieces - init_state.blackPieces << "\t";
	cout << a1.num_moves << "\t" << a2.num_moves << "\t";
	cout << a1.nodes << "\t" << a2.nodes << "\t";
	cout << (double)a1.nodes / (double)a1.num_moves << "\t" << (double)a2.nodes / (double)a2.num_moves << "\t";
	cout << a1.total_time / (double)a1.num_moves << "\t" << a2.total_time / (double)a2.num_moves << endl;
    return 0;
}

