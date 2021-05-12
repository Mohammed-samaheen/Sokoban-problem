#pragma once

#include "SokoPan.h"

SokoPan::SokoPan(vector<vector<char>> maze) {
	this->MAZE = maze;
	fillMazeOut(MAZE);
	fill_R();

}

vector<vector<char>> SokoPan::getUpdatedMaze(Grid grid, Position::Move move) {

	Position man = grid.getMan();
	if (man.col < 0) return {};

	Position newMan = man + move;

	vector<vector<char>> maze = grid.getMaze();

	if (__Cell(maze, newMan) == BOX || __Cell(maze, newMan) == BOX_GOAL) {

		Position nextNewMan = newMan + move;

		if (__Cell(maze, nextNewMan) == EMPTY) {
			__Cell(maze, nextNewMan) = BOX;
		}
		else if (__Cell(maze, nextNewMan) == GOAL) {
			__Cell(maze, nextNewMan) = BOX_GOAL;
		}
		else {
			return maze; //TODO: check if not return maze
		}

		if (__Cell(maze, newMan) == BOX) {
			__Cell(maze, newMan) = EMPTY;
		}
		else {
			__Cell(maze, newMan) = GOAL;
		}
	}

	if (__Cell(maze, newMan) == EMPTY) {
		__Cell(maze, newMan) = MAN;
	}

	else if (__Cell(maze, newMan) == GOAL) {
		__Cell(maze, newMan) = MAN_GOAL;
	}

	else {
		return maze; // TODO: check not return maze
	}

	if (__Cell(maze, man) == MAN) {
		__Cell(maze, man) = EMPTY;
	}

	else {
		__Cell(maze, man) = GOAL;
	}

	return maze;
}

// return maze without man position
vector<vector<char>> SokoPan::getAbstractMaze(Grid grid) {

	if (grid.getMan().row == -1) return grid.getMaze();

	vector<vector<char>> maze = grid.getMaze();
	__Cell(maze, grid.getMan()) = EMPTY;

	return maze;
}

map < vector<vector<char>>, map<char, double> > SokoPan::train(sf::RenderWindow& window, bool isDraw) {
	
	int episodes = 500;//EPISODES;
	while (episodes--) {
	
		int maxLoops = 1000;
		Grid state = Grid(generateRandomState());
	
		cout << episodes << endl;
		while (!isFinalGoal(state) && !state.hasDeadlock() && maxLoops--) {

			map<char, Grid> allActionState = getAllActionStates(state);
			if (allActionState.size() == 0) {
				episodes++;
				break;
			}

			pair<char, Grid> actionsState = getRandomPossibleAction(allActionState);
			char action = actionsState.first;
			Grid nextState = actionsState.second;

			double max_q = calcTraingProfit(nextState.getMaze());
			double profit = R[state.getMaze()][action] + TRAINING_FACTOR * max_q;

			Q[state.getMaze()][action] = profit;

			if (isDraw) {
				Utils::draw(window, state.getMaze());

			}

			state = nextState;

		}
	}

	return Q;
}

void SokoPan::solve(vector<vector<char>> maze) {
	Grid grid = Grid(maze);
	Position man = grid.getMan();
	string dir = "udlr";
	while (!isFinalGoal(grid)) {
		maze = grid.getMaze();
		int maxi = -100;
		char best = 'u';
		for (char move : dir) {
			cout << move << " " << Q[maze][move] << endl;
			if (Q[maze][move] >= maxi) {
				best = move;
				maxi = Q[maze][move];
			}
		}

		grid = Grid(getUpdatedMaze(grid, Position::Move(best)));
		Utils::p2d(grid.getMaze());
	}
}

void SokoPan::printQ() {
	for (auto it : Q) {
		for (auto i : it.second) {
			cout << i.first << " " << i.second << endl;
			Utils::p2d(it.first);
		}
	}
}

void SokoPan::printR() {
	for (auto it : R) {
		for (auto i : it.second) {
			if (i.second > 0) {
				cout << i.second << endl;
				Utils::p2d(it.first);
			}
		}
	}
}

vector<vector<char>> SokoPan::getNextFram(map < vector<vector<char>>, map<char, double> >& Q, sf::RenderWindow& window, vector<vector<char>> maze) {

	Grid grid = Grid(maze);
	string dir = "udlr";

	double maxi = INT_MIN;
	char best = 'u';
	for (char move : dir) {
		cout << move << " " << Q[maze][move] << endl;
		if (Q[maze][move] >= maxi) {
			best = move;
			maxi = Q[maze][move];
		}
	}

	grid = Grid(getUpdatedMaze(grid, Position::Move(best)));
	cout << "$$$$$$$$$$$" << endl;
	Utils::p2d(grid.getMaze());
	Utils::draw(window, grid.getMaze());
	return grid.getMaze();
}

bool SokoPan::isInvalidMove(Grid grid, Position::Move move) {
	Position pos = grid.getMan() + move;
	vector<vector<char>> maze = grid.getMaze();
	return grid.isOutBorder(pos) || Grid::isTypeCell(maze, pos, { WALL, OUT });
}

void SokoPan::floodFill(vector<vector<char>>& grid, Position pos) {

	if (!Grid::isTypeCell(grid, pos, EMPTY)) return;
	__Cell(grid, pos) = OUT;

	string dir = "udlr";
	for (char ch : dir) {
		floodFill(grid, pos + Position::Move(ch));
	}

}

void SokoPan::fillMazeOut(vector<vector<char>>& maze) {
	for (int i = 0; i < maze.size(); i++) {
		floodFill(maze, Position(i, 0));
		floodFill(maze, Position(i, maze[0].size() - 1));
	}
	for (int i = 0; i < maze[0].size(); i++) {
		floodFill(maze, Position(0, i));
		floodFill(maze, Position(maze.size() - 1, i));
	}
}

bool SokoPan::isFinalGoal(Grid& grid) {
	if (!grid.isState()) return false;
	vector<vector<char>> maze = grid.getMaze();
	for (const auto& row : maze) {
		for (const auto& cell : row) {
			if (cell == GOAL || cell == BOX) {
				return false;
			}
		}
	}
	return true;
}

Grid SokoPan::getActionState(Grid& state, char action) {

	vector<vector<char>> maze = state.getMaze();

	Position::Move move = Position::Move(action);
	if (isInvalidMove(state, move)) {
		return Grid();
	}

	maze = getUpdatedMaze(state, move);

	if (state == maze)
		return Grid();
	return Grid(maze);
}

map<char, Grid> SokoPan::getAllActionStates(Grid& grid) {
	string actions = "udlr";
	map<char, Grid> actionStates;
	for (char action : actions) {
		Grid state = getActionState(grid, action);
		if (state.isState() && !state.hasDeadlock()) {
			actionStates[action] = state;
		}
		/*else {
			R[grid.getMaze()][action] = INVALID_COAST;
		}*/
	}
	return actionStates;
}

double SokoPan::calcTraingProfit(vector<vector<char>> n_state) {

	Grid grid = Grid(n_state);

	double maxi = 0;
	string actions = "udlr";

	for (const auto& action : Q[n_state]) {
		double val_q = action.second;
		maxi = max(maxi, val_q);
	}



	return maxi;
}

vector<vector<char>> SokoPan::generateGoalState(vector<vector<char>> maze) {
	for (int row = 0; row < maze.size(); row++) {
		for (int col = 0; col < maze[row].size(); col++) {
			// TODO: check if
			if (maze[row][col] == MAN || maze[row][col] == BOX)
				maze[row][col] = EMPTY;
			else if (maze[row][col] == GOAL)
				maze[row][col] = BOX_GOAL;
		}
	}
	return maze;
}

vector<vector<char>> SokoPan::tryReachGoal(vector<vector<char>> state, Position pos, char ch) {
	Position::Move move = Position::Move(ch);
	Position prePos = pos + move;
	if (Grid::isTypeCell(state, prePos, EMPTY)) {
		Position pre_2Pos = prePos + move;
		if (Grid::isTypeCell(state, pre_2Pos, EMPTY)) {

			__Cell(state, pos) = GOAL;
			__Cell(state, prePos) = BOX;
			__Cell(state, pre_2Pos) = MAN;
			return state;
		}
	}
	return {};
}

void SokoPan::fill_R() {
	string direction = "udlr";
	string invDirection = "durl";

	vector<vector<char>> goalState = generateGoalState(MAZE);
	for (int row = 0; row < goalState.size(); row++) {
		for (int col = 0; col < goalState[row].size(); col++) {

			if (goalState[row][col] == BOX_GOAL) {
				for (int i = 0; i < direction.size(); i++) {

					vector<vector<char>> preState = tryReachGoal(goalState, Position(row, col), direction[i]);
					if (preState.size() == 0) continue;
					char preMove = invDirection[i];
					R[preState][preMove] = PROFIT_GOAL;

				}
			}

		}
	}
	return;
}

vector<vector<char>> SokoPan::generateRandomState() {

	vector<vector<char>> maze = getAbstractMaze(Grid(MAZE));

	if (maze.size() == 0) return {};

	Position pos = Position();
	do {
		int row = (rand() % maze.size());
		int col = (rand() % maze[0].size());
		pos = Position(row, col);
	} while (!Grid::isTypeCell(maze, pos, EMPTY));

	__Cell(maze, pos) = MAN;

	return maze;
}

pair<char, Grid> SokoPan::getRandomPossibleAction(map<char, Grid>& allActions) {
	if (allActions.size() == 0) {
		return make_pair(' ', Grid());
	}
	string actions = "udlr";
	while (true) {
		int idx = rand() % actions.size();
		char action = actions[idx];
		if (allActions.find(action) != allActions.end()/* && R[allActions[action].getMaze()][action] != INVALID_COAST */ && allActions[action].isState()) {
			return make_pair(action, allActions[action]);
		}
	}
}
