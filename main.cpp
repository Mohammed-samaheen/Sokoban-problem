#include <iostream>
#include <string>
#include <vector>
#include <map>


using namespace std;

typedef vector<vector<char>> vvc;

int const MAZE_SIZE = 9;
const char MAN = 'M';
const char BOX = 'B';
const char WALL = 'W';
const char GOAL = 'G';
const char EMPTY = ' ';
const char MAN_GOAL = 'Z';
const char BOX_GOAL = 'X';
const float TRAINING_FACTOR = 0.8;

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
		//vvc maze = vvc(size, vector<char>(size, ' '));
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
					isDeadlock(row, col);
				}
			}
		}
	}

	Position getMan() {
		return man;
	}

private:
	
	vvc maze;
	Position man;

	bool isDeadlock(int row, int col) {
		
		if ((isBorder(row - 1, col) || maze[row - 1][col] == WALL)) {
			row = row - 1;
			return (isBorder(row, col - 1) || maze[row][col - 1] == WALL) || (isBorder(row, col + 1) || maze[row][col + 1] == WALL);
		}
		
		else if ((isBorder(row + 1, col) || maze[row + 1][col] == WALL)) {
			row = row + 1;
			return (isBorder(row, col - 1) || maze[row][col - 1] == WALL) || (isBorder(row, col + 1) || maze[row][col + 1] == WALL);
		}

		return false;
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


bool isInvalidMove(Grid& grid, Position::Move move) {
	Position pos = grid.getMan() + move;
	vvc maze = grid.getMaze();
	return pos.row < 0 || pos.col < 0 || pos.row >= maze.size() || pos.row >= maze.back().size() || maze[pos.row][pos.col] == WALL;
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
	}
	return actionStates;
}

// TOMOD:
// TODO: Modify R to handle unvaild state and deadlock state somewhere
float getNextQ_val(vvc pre_state, char pre_action) {
	float maxi = 0;

	vvc state = getUpdatedMaze(Grid(pre_state), Position::Move(pre_action));

	for (const auto& action : Q[state]) {
		float val_q = Q[state][action.first];
		max(maxi, val_q);
	}

	return R[pre_state][pre_action] + TRAINING_FACTOR * maxi;
}

//TODO:
void fill_R() {
	return;
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