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

using namespace LibConstants;

#define __Cell(grid, pos) grid[pos.row][pos.col]


class SokoPan {

public:

	SokoPan(vector<vector<char>> maze);

	vector<vector<char>> static getUpdatedMaze(Grid grid, Position::Move move);

	// return maze without man position
	vector<vector<char>> static getAbstractMaze(Grid grid);

	map < vector<vector<char>>, map<char, double> > train(sf::RenderWindow& window, bool isDraw);

	void solve(vector<vector<char>> maze);

	void printQ();

	void printR();

	vector<vector<char>> static getNextFram(map < vector<vector<char>>, map<char, double> >& Q, sf::RenderWindow& window, vector<vector<char>> maze);

	bool isFinalGoal(Grid& grid);

private:

	vector<vector<char>> MAZE;

	map < vector<vector<char>>, map<char, double> > Q;
	map < vector<vector<char>>, map<char, double> > R;

	bool isInvalidMove(Grid grid, Position::Move move);

	void floodFill(vector<vector<char>>& grid, Position pos);

	void fillMazeOut(vector<vector<char>>& maze);

	Grid getActionState(Grid& state, char action);

	map<char, Grid> getAllActionStates(Grid& grid);

	double calcTraingProfit(vector<vector<char>> n_state);

	vector<vector<char>> generateGoalState(vector<vector<char>> maze);

	vector<vector<char>> tryReachGoal(vector<vector<char>> state, Position pos, char ch);

	void fill_R();

	vector<vector<char>> generateRandomState();

	pair<char, Grid> getRandomPossibleAction(map<char, Grid>& allActions);

};
