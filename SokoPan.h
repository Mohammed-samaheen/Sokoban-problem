#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Position.h"
#include "Utils.h"
#include "Grid.h"
#include "LibConstant.hpp"

using namespace std;

typedef vector<vector<char>> vvc;
using namespace LibConstants;

#define __Cell(grid, pos) grid[pos.row][pos.col]


class SokoPan {

public:

	SokoPan(vvc maze);

	vvc static getUpdatedMaze(Grid grid, Position::Move move);

	// return maze without man position
	vvc static getAbstractMaze(Grid grid);

	map < vvc, map<char, double> > train(sf::RenderWindow& window, bool isDraw);

	void solve(vvc maze);

	void printQ();

	void printR();

	vvc static getNextFram(map < vvc, map<char, double> >& Q, sf::RenderWindow& window, vvc maze);

	bool isFinalGoal(Grid& grid);

private:

	vvc MAZE;

	map < vvc, map<char, double> > Q;
	map < vvc, map<char, double> > R;

	bool isInvalidMove(Grid grid, Position::Move move);

	void floodFill(vvc& grid, Position pos);

	void fillMazeOut(vvc& maze);

	Grid getActionState(Grid& state, char action);

	map<char, Grid> getAllActionStates(Grid& grid);

	double calcTraingProfit(vvc n_state);

	vvc generateGoalState(vvc maze);

	vvc tryReachGoal(vvc state, Position pos, char ch);

	void fill_R();

	vvc generateRandomState();

	pair<char, Grid> getRandomPossibleAction(map<char, Grid>& allActions);

};
