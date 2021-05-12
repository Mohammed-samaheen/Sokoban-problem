#pragma once

#ifndef MYLIB_CONSTANTS_H
#define MYLIB_CONSTANTS_H

typedef vector<vector<char>> vvc;



//  File Name : LibConstants.hpp    Purpose : Global Constants for Lib Utils
namespace LibConstants
{
	
	int const MAZE_SIZE = 9;
	const char MAN = 'M';
	const char BOX = 'B';
	const char WALL = 'W';
	const char GOAL = 'G';
	const char EMPTY = ' ';
	const char MAN_GOAL = 'Z';
	const char BOX_GOAL = 'X';
	const char OUT = 'O';

	const int EPISODES = 1000;
	const int INVALID_COAST = -1;

	const float TRAINING_FACTOR = 0.8;
	const float PROFIT_GOAL = 1000.0;


}
#endif 