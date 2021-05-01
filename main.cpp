#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

typedef vector<vector<char>> vvc;

#define __Cell(grid, pos) grid[pos.row][pos.col]

int const MAZE_SIZE = 9;
const char MAN = 'M';
const char BOX = 'B';
const char WALL = 'W';
const char GOAL = 'G';
const char EMPTY = ' ';
const char MAN_GOAL = 'Z';
const char BOX_GOAL = 'X';
const char OUT = 'O';

const int EPISODES = 10;
const int INVALID_COAST = -1;

const float TRAINING_FACTOR = 0.8;
const float PROFIT_GOAL = 100.0;

map < vvc, map<char, float> > Q;
map < vvc, map<char, float> > R;

class Position {

public:

	int row;
	int col;

	Position() {
		Position(0, 0);
	}

	Position(int x, int y) {
		this->row = x;
		this->col = y;
	}

	struct Move {

	public:
		Move(int mx, int my) {
			this->move_row = mx;
			this->move_col = my;
		}

		Move(char ch) {
			switch (ch) {
			case 'u': { Move(-1, 0); break; }
			case 'd': { Move(1, 0); break; }
			case 'l': { Move(0, -1); break; }
			case 'r': { Move(0, 1); break; }
			default: { Move(0, 0); }
			}
		}

		Move operator+ (Move recivedMove) {
			return Move(this->move_row + recivedMove.move_row, this->move_col + recivedMove.move_col);
		}

		int move_row;
		int move_col;
	};

	Position operator+ (Move move) {
		return Position(this->row + move.move_row, this->col + move.move_col);
	}

};

class Grid {

public:
	Grid(int size) {
		Grid(vvc(size, vector<char>(size, ' ')));
	}

	Grid() {
		Grid({});
	}

	Grid(vvc maze) {
		this->maze = maze;
		getManPosition();
	}

	vvc getMaze() {
		return this->maze;
	}

	bool operator==(vvc& recivedMaze) {

		if (maze.size() != recivedMaze.size() || (maze.size() > 0 && maze[0].size() != recivedMaze[0].size())) {
			return false;
		}

		for (int row = 0; row < maze.size(); row++) {
			for (int col = 0; col < maze[row].size(); col++) {
				if (maze[row][col] != recivedMaze[row][col]) {
					return false;
				}
			}
		}

		return true;
	}

	bool isState() {
		return this->maze.size() > 0 && this->maze[0].size() > 0;
	}

	bool hasDeadlock() {
		for (int row = 0; row < maze.size(); row++) {
			for (int col = 0; col < maze[row].size(); col++) {
				if (maze[row][col] == BOX) {
					if (isDeadlock(row, col)) {
						return true;
					}
				}
			}
		}
		return false;
	}

	Position getMan() {
		return man;
	}

private:
	
	vvc maze;
	Position man;

	bool isDeadlock(int row, int col) {

		bool res = false;
		if ((isBorder(row - 1, col) || maze[row - 1][col] == WALL)) {
			row--;
			res = res || (isBorder(row, col - 1) || maze[row][col - 1] == WALL) || (isBorder(row, col + 1) || maze[row][col + 1] == WALL);
		}
		
		else if ((isBorder(row + 1, col) || maze[row + 1][col] == WALL)) {
			row++;
			res = res || (isBorder(row, col - 1) || maze[row][col - 1] == WALL) || (isBorder(row, col + 1) || maze[row][col + 1] == WALL);
		}

		return res;
	}

	char isBorder(int row, int col) {
		return (row < 0 || col < 0 || row >= maze.size() || maze.size() == 0 || col >= maze[0].size());
	}

	void getManPosition() {

		this->man = Position(-1, -1);

		for (int row = 0; row < this->maze.size(); row++) {
			for (int col = 0; col < this->maze[row].size(); col++) {
				if (this->maze[row][col] == MAN || this->maze[row][col] == MAN_GOAL) {
					this->man = Position(row, col);
				}
			}
		}
	}

};

vvc MAZE = { {' ',' ',' ','W','W','W','W','W',' '},
						   {' ','W','W','W',' ',' ',' ','W',' '},
						   {' ','W','G',' ','B',' ',' ','W',' '},
						   {' ','W','W','W',' ','B','G','W',' '},
						   {' ','W','G','W','W','B',' ','W',' '},
						   {' ','W',' ','W',' ','G',' ','W','W'},
						   {' ','W','B','B','G','B','B','G','W'},
						   {' ','W',' ',' ',' ','G',' ',' ','W'},
						   {' ','W','W','W','W','W','W','W','W'} };

// TODO: test MACRO
vvc getUpdatedMaze(Grid grid, Position::Move move) {

	Position man = grid.getMan();
	Position newMan = man + move;

	vvc maze = grid.getMaze();

	if (__Cell(maze, newMan) == BOX || __Cell(maze, newMan) == BOX_GOAL) {

		Position nextNewMan = newMan + move;

		if (__Cell(maze, nextNewMan) == EMPTY) {
			__Cell(maze, nextNewMan) = BOX;
		}
		else if (__Cell(maze, nextNewMan) == GOAL) {
			__Cell(maze, nextNewMan) = BOX_GOAL;
		}
		else {
			return {};
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
		return {};
	}

	if (__Cell(maze, man) == MAN) {
		__Cell(maze, man) = EMPTY;
	}

	else {
		__Cell(maze, man) = GOAL;
	}

	return maze;
}

/*
vvc getUpdatedMaze(Grid grid, Position::Move move) {

	Position man = grid.getMan();
	Position newMan = grid.getMan() + move;
	int row = man.row, n_row = newMan.row;
	int col = man.col, n_col = newMan.col;

	vvc maze = grid.getMaze();

	if (maze[n_row][n_col] == BOX || maze[n_row][n_col] == BOX_GOAL) {

		int nn_row = n_row + move.move_row;
		int nn_col = n_col + move.move_col;

		if (maze[nn_row][nn_col] == EMPTY) {
			maze[nn_row][nn_col] = BOX;
		}
		else if (maze[nn_row][nn_col] == GOAL) {
			maze[nn_row][nn_col] = BOX_GOAL;
		}
		else {
			return {};
		}

		if (maze[n_row][n_col] == BOX) {
			maze[n_row][n_col] = EMPTY;
		}
		else {
			maze[n_row][n_col] = GOAL;
		}
	}

	if (maze[n_row][n_col] == EMPTY) {
		maze[n_row][n_col] = MAN;
	}

	else if (maze[n_row][n_col] == GOAL) {
		maze[n_row][n_col] = MAN_GOAL;
	}

	else {
		return {};
	}

	if (maze[row][col] == MAN) {
		maze[row][col] = EMPTY;
	}

	else {
		maze[row][col] = GOAL;
	}

	return maze;
}
*/

bool isInvalidMove(Grid grid, Position::Move move) {
	Position pos = grid.getMan() + move;
	vvc maze = grid.getMaze();
	return pos.row < 0 || pos.col < 0 || pos.row >= maze.size() || (maze.size() > 0 && pos.row >= maze[0].size()) || __Cell(maze, pos) == WALL;
}

bool isTypeCell(vvc state, Position pos, char type) {
	return !(pos.row < 0 || pos.col < 0 || pos.row >= state.size() || (state.size() > 0 && pos.row >= state[0].size())) && __Cell(state, pos) == type;

}

Grid getActionState(Grid& state, char action) {

	vvc maze = state.getMaze();

	Position::Move move = Position::Move(action);
	if (isInvalidMove(state, move)) {
		return Grid();
	}
	
	maze = getUpdatedMaze(state, move);
	return Grid(maze);
}

map<char, Grid> getAllActionStates(Grid& grid) {
	string actions = "udlr";
	map<char, Grid> actionStates;
	for (char action : actions) {
		Grid state = getActionState(grid, action);
		if (state.isState()) {
			actionStates[action] = state;
		}
		else {
			R[grid.getMaze()][action] = INVALID_COAST;
		}
	}
	return actionStates;
}

float calcTraingProfit(vvc current_state, char current_action) {
	float maxi = 0;
	
	vvc state = getUpdatedMaze(Grid(current_state), Position::Move(current_action));
	Grid grid = Grid(state);

	if (R[current_state][current_action] == INVALID_COAST || grid.hasDeadlock() || !grid.isState()) {
		R[current_state][current_action] = INVALID_COAST;
	}
	
	else {
		for (const auto& action : Q[state]) {
			float val_q = Q[state][action.first];
			max(maxi, val_q);
		}
	}

	return R[current_state][current_action] + TRAINING_FACTOR * maxi;
}

vvc generateGoalState(vvc maze) {
	for (int row = 0; row < maze.size(); row++)	{
		for (int col = 0; col < maze[row].size(); col++) {
			if (maze[row][col] || maze[row][col] == BOX)
				maze[row][col] = EMPTY;
			else if (maze[row][col] == GOAL)
				maze[row][col] = BOX_GOAL;
		}
	}
	return maze;
}

vvc tryReachGoal(vvc state, Position pos, char ch) {
	Position::Move move = Position::Move(ch);
	Position prePos = pos + move;
	if (isTypeCell(state, prePos, EMPTY)) {
		Position pre_2Pos = prePos + move;
		if (isTypeCell(state, pre_2Pos, EMPTY)) {

			__Cell(state, pos) = GOAL;
			__Cell(state, prePos) = BOX;
			__Cell(state, pre_2Pos) = MAN;
			/*
			state[pos.row][pos.col] = GOAL;
			state[prePos.row][prePos.col] = BOX;
			state[pre_2Pos.row][pre_2Pos.col] = MAN;
			*/
			return state;
		}
	}
	return {};
}

void fill_R() {
	string direction = "udlr";
	string invDirection = "durl";

	vvc goalState = generateGoalState(MAZE);
	for (int row = 0; row < goalState.size(); row++) {
		for (int col = 0; col < goalState[row].size(); col++) {
			if (goalState[row][col] == BOX_GOAL) {
				for (int i = 0; i < direction.size(); i++) {
					vvc preState = tryReachGoal(goalState, Position(row, col), direction[i]);
					if (preState.size() == 0) continue;
					char preMove = invDirection[i];
					R[preState][preMove] = PROFIT_GOAL;
				}
			}
		}
	}
	return;
}

bool isFinalGoal(Grid& grid) {
	vvc maze = grid.getMaze();
	for (const auto& row : maze) {
		for (const auto& cell : row) {
			if (cell == GOAL) {
				return false;
			}
		}
	}
	return true;
}

vvc getAbstractMaze(Grid grid) {
	if (!grid.isState()) return {};
	vvc maze = grid.getMaze();
	__Cell(maze, grid.getMan()) = EMPTY;
	return maze;
}

vvc generateRandomState() {
	
	vvc maze = getAbstractMaze(Grid(MAZE));

	if (maze.size() == 0) return {};

	Position pos = Position();
	do {
		int row = (rand() % maze.size());
		int col = (rand() % maze[0].size());
		pos = Position(row, col);
	} while (!isTypeCell(maze, pos, EMPTY));

	__Cell(maze, pos) = MAN;

	return maze;
}

void floodFill(vvc& grid, Position pos) {
	
	if (!isTypeCell(grid, pos, EMPTY)) return;
	__Cell(grid, pos) = OUT;
	
	string dir = "udlr";
	for (char ch : dir) {
		floodFill(grid, pos + Position::Move(ch));
	}

}

pair<char, Grid> getRandomPossibleAction(map<char, Grid>& allActions) {
	if (allActions.size() == 0) {
		return make_pair(' ', Grid());
	}
	string actions = "udlr";
	while (true) {
		int idx = rand() % actions.size();
		char action = actions[idx];
		if (allActions.find(action) != allActions.end() && R[allActions[action].getMaze()][action] != INVALID_COAST) {
			return make_pair(action, allActions[action]);
		}
	}
}

void run() {
	int episodes = EPISODES;

	while (episodes--) {
		Grid state = Grid(generateRandomState());
		while (!isFinalGoal(state) && !state.hasDeadlock()) {
			
			map<char, Grid> allActionState = getAllActionStates(state);
			pair<char, Grid> actionsState = getRandomPossibleAction(allActionState);
			char action = actionsState.first;
			Grid nextState = actionsState.second;

			float profit = calcTraingProfit(nextState.getMaze(), action);
			Q[state.getMaze()][action] = profit;

			state = nextState;
		}
	}

}






















vector<pair<int, int> > goalLocation;
bool invalidPoint(int x, int y);
vector<pair<int, int> > generateNextStates(int x, int y);
void findGoalLocation();

int main()
{
	findGoalLocation();

	return 0;
}

// find location (x,y) of each goal point
void findGoalLocation() {
	for (int i = 0; i < MAZE_SIZE; ++i)
		for (int j = 0; j < MAZE_SIZE; ++j)
			if (MAZE[i][j] == 'G')
				goalLocation.emplace_back(i, j);
}

vector<pair<int, int> > generateNextStates(int x, int y) {
	vector<pair<int, int> > validState;
	int x_move[] = { 0,1,-1,0 };
	int y_move[] = { 1,0,0,-1 };

	for (int i = 0; i < 4; ++i) {
		int next_x = x + x_move[i];
		int next_y = y + y_move[i];

		if (invalidPoint(next_x, next_y))
			continue;
		if (MAZE[next_x][next_y] == 'B') {
			int _x = x + x_move[i];
			int _y = y + y_move[i];

			if (invalidPoint(_x, _y) || MAZE[_x][_y] == 'B')
				continue;
		}
		validState.emplace_back(next_x, next_y);
	}

	return validState;
}

bool invalidPoint(int x, int y) {
	return x >= MAZE_SIZE || y >= MAZE_SIZE || MAZE[x][y] == 'W';
}

bool isEndState(int x, int y) {
	for (auto& i : goalLocation)
		if (MAZE[i.first][i.second] != 'X')
			return false;

	return true;
}