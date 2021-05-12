#pragma once

#include "LibConstant.hpp"
#include <string>
#include <vector>
#include <map>
#include "Position.h" 
#include "LibConstant.hpp"

using namespace std;
using namespace LibConstants;

#define __Cell(grid, pos) grid[pos.row][pos.col]
 


class Grid {

public:
	Grid(int size);

	Grid();

	Grid(vector<vector<char>> maze);

	vector<vector<char>> getMaze();

	bool operator==(vector<vector<char>>& recivedMaze);

	bool isState();

	bool hasDeadlock();

	Position getMan();

	bool isOutBorder(Position pos);

	bool isOutBorder(int row, int col);

	bool static isTypeCell(vector<vector<char>> state, Position pos, char type);

	bool static isTypeCell(vector<vector<char>> state, Position pos, vector<char> types);

private:

	vector<vector<char>> maze;
	Position man;

	bool isDeadlock(int row, int col);

	void getManPosition();

};
