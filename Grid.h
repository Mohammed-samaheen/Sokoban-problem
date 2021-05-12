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
typedef vector<vector<char>> vvc;


class Grid {

public:
	Grid(int size);

	Grid();

	Grid(vvc maze);

	vvc getMaze();

	bool operator==(vvc& recivedMaze);

	bool isState();

	bool hasDeadlock();

	Position getMan();

	bool isOutBorder(Position pos);

	bool isOutBorder(int row, int col);

	bool static isTypeCell(vvc state, Position pos, char type);

	bool static isTypeCell(vvc state, Position pos, vector<char> types);

private:

	vvc maze;
	Position man;

	bool isDeadlock(int row, int col);

	void getManPosition();

};
