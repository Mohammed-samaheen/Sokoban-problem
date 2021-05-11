#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef vector<vector<char>> vvc;
#define float long double


sf::RenderWindow window(sf::VideoMode(500, 500), "Sokoban");
sf::RectangleShape rectangle(sf::Vector2f(19, 19));
sf::CircleShape circle(8);
sf::CircleShape triangle(10, 6);

//TODO
void printQ();
void printR();
void p2d(vvc maze);

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

const int EPISODES = 1000;
const int INVALID_COAST = -1;

const float TRAINING_FACTOR = 0.8;
const float PROFIT_GOAL = 1000.0;

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
			case 'u': { new (this) Move(-1, 0); break; }
			case 'd': { new (this) Move(1, 0); break; }
			case 'l': { new (this) Move(0, -1); break; }
			case 'r': { new (this) Move(0, 1); break; }
			default: { new (this) Move(0, 0); }
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
bool isTypeCell(vvc state, Position pos, char type);
bool isTypeCell(vvc state, Position pos, vector<char> types);

class Grid {

public:
	Grid(int size) {
		new (this) Grid(vvc(size, vector<char>(size, ' ')));
	}

	Grid() {
		new (this) Grid({});
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

	bool isOutBorder(Position pos) {
		return isOutBorder(pos.row, pos.col);
	}

	bool isOutBorder(int row, int col) {
		return (row < 0 || col < 0 || row >= maze.size() || maze.size() == 0 || col >= maze[0].size());
	}

private:
	
	vvc maze;
	Position man;

	bool isDeadlock(int row, int col) {

		bool res = true;

		Position pos = Position(row, col);

		Position up = pos + Position::Move('u');
		Position right = pos + Position::Move('r');
		Position left = pos + Position::Move('l');
		Position down = pos + Position::Move('d');

		if (isOutBorder(up) || isTypeCell(maze, up, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
			if (isOutBorder(left) || isTypeCell(maze, left, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
				return true;
			}
			if (isOutBorder(right) || isTypeCell(maze, right, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
				return true;
			}
		}

		if (isOutBorder(down) || isTypeCell(maze, down, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
			if (isOutBorder(left) || isTypeCell(maze, left, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
				return true;
			}
			if (isOutBorder(right) || isTypeCell(maze, right, { WALL/*, BOX, BOX_GOAL*/, OUT })) {
				return true;
			}
		}

		return false;
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

vvc MAZE =				 { {' ',' ',' ','W','W','W','W','W',' '},
						   {' ','W','W','W',' ',' ',' ','W',' '},
						   {' ','W','G',' ','B',' ',' ','W',' '},
						   {' ','W','W','W','M','B','G','W',' '},
						   {' ','W','G','W','W','B',' ','W',' '},
						   {' ','W',' ','W',' ','G',' ','W','W'},
						   {' ','W','B','B','G','B','B','G','W'},
						   {' ','W',' ',' ',' ','G',' ',' ','W'},
						   {' ','W','W','W','W','W','W','W','W'} };

vvc MAZE2 =
{ {' ',' ',' ','W','W','W','W','W',' '},
  {' ','W','W','W','W','W','W','W',' '},
  {' ','W','W','G',' ',' ',' ','W',' '},
  {' ','W','W',' ','W',' ',' ','W',' '},
  {' ','W','W','M','B',' ','W','W',' '},
  {' ','W','W','W',' ',' ','W','W','W'},
  {' ','W','G','B','B','W','W','W','W'},
  {' ','W','W','W','G','W','W','W','W'},
  {' ','W','W','W','W','W','W','W','W'} };


vvc MAZE4 =
{ {' ',' ',' ','W','W','W','W','W',' '},
  {' ','W','W','W','W','W','W','W',' '},
  {' ','W','W','G',' ',' ',' ','W',' '},
  {' ','W','W',' ','W',' ',' ','W',' '},
  {' ','W','W','M','B',' ','W','W',' '},
  {' ','W','W','W',' ',' ','W','W','W'},
  {' ','W','G','B','B','B','B','G','W'},
  {' ','W','W','W','G','G','W','W','W'},
  {' ','W','W','W','W','W','W','W','W'} };

vvc MAZE3 =
{{'W','W','W','W','W','W','W','W','W'},
 {'W','W','W','W','W','W','W','W','W'},
 {'W','W','W','W','W','W','W','W','W'},
 {'W','W',' ',' ',' ','W','M',' ','W'},
 {'W','W',' ',' ',' ',' ','B',' ','W'},
 {'W','W',' ',' ',' ','W',' ',' ','W'},
 {'W','W','W','W','W','W','B','G','W'},
 {'W','W','W','W','W','W',' ','G','W'},
 {'W','W','W','W','W','W','W','W','W'} };


vvc getUpdatedMaze(Grid grid, Position::Move move) {

	Position man = grid.getMan();
	if (man.col < 0) return {};

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

bool isInvalidMove(Grid grid, Position::Move move) {
	Position pos = grid.getMan() + move;
	vvc maze = grid.getMaze();
	return grid.isOutBorder(pos) || isTypeCell(maze, pos, {WALL, OUT});
}

bool isTypeCell(vvc state, Position pos, char type) {
	
	if (Grid(state).isOutBorder(pos)) {
		return false;
	}

	return  __Cell(state, pos) == type;
}

bool isTypeCell(vvc state, Position pos, vector<char> types) {
	for (char type : types) {
		if (isTypeCell(state, pos, type)) {
			return true;
		}
	}
	return false;
}

void p2d(vvc v) {
	for (auto it : v) {
		for (auto a : it) {
			cout << a << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void floodFill(vvc& grid, Position pos) {

	if (!isTypeCell(grid, pos, EMPTY)) return;
	__Cell(grid, pos) = OUT;

	string dir = "udlr";
	for (char ch : dir) {
		floodFill(grid, pos + Position::Move(ch));
	}

}

void fillMazeOut(vvc& maze) {
	for (int i = 0; i < maze.size(); i++) {
		floodFill(maze, Position(i, 0));
		floodFill(maze, Position(i, maze[0].size() - 1));
	}
	for (int i = 0; i < maze[0].size(); i++) {
		floodFill(maze, Position(0, i));
		floodFill(maze, Position(maze.size() - 1, i));
	}
}

bool isFinalGoal(Grid& grid) {
	if (!grid.isState()) return false;
	vvc maze = grid.getMaze();
	for (const auto& row : maze) {
		for (const auto& cell : row) {
			if (cell == GOAL || cell == BOX) {
				return false;
			}
		}
	}
	return true;
}

// return maze without man position
vvc getAbstractMaze(Grid grid) {
	
	if (grid.getMan().row == -1) return grid.getMaze();
	
	vvc maze = grid.getMaze();
	__Cell(maze, grid.getMan()) = EMPTY;
	
	return maze;
}

Grid getActionState(Grid& state, char action) {

	vvc maze = state.getMaze();

	Position::Move move = Position::Move(action);
	if (isInvalidMove(state, move)) {
		return Grid();
	}
	
	maze = getUpdatedMaze(state, move);

	if (state == maze)
		return Grid();
	return Grid(maze);
}

map<char, Grid> getAllActionStates(Grid& grid) {
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


float calcTraingProfit(vvc n_state) {
	
	Grid grid = Grid(n_state);

	float maxi = 0;
	string actions = "udlr";
	
	for (const auto& action : Q[n_state]) {
		float val_q = action.second;
		maxi = max(maxi, val_q);
	}



	return maxi; 
}

vvc generateGoalState(vvc maze) {
	for (int row = 0; row < maze.size(); row++)	{
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

vvc tryReachGoal(vvc state, Position pos, char ch) {
	Position::Move move = Position::Move(ch);
	Position prePos = pos + move;
	if (isTypeCell(state, prePos, EMPTY)) {
		Position pre_2Pos = prePos + move;
		if (isTypeCell(state, pre_2Pos, EMPTY)) {

			__Cell(state, pos) = GOAL;
			__Cell(state, prePos) = BOX;
			__Cell(state, pre_2Pos) = MAN;
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

pair<char, Grid> getRandomPossibleAction(map<char, Grid>& allActions) {
	if (allActions.size() == 0) {
		return make_pair(' ', Grid());
	}
	string actions = "udlr";
	while (true) {
		int idx = rand() % actions.size();
		char action = actions[idx];
		if (allActions.find(action) != allActions.end()/* && R[allActions[action].getMaze()][action] != INVALID_COAST */&& allActions[action].isState()) {
			return make_pair(action, allActions[action]);
		}
	}
}

void run() {
	int episodes = 100;//EPISODES;
	while (episodes--) {
		int maxLoops = 500;
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
			if (isFinalGoal(nextState)) {
				cout << action << "^^^^^^" << Q[state.getMaze()][action] << endl;
				p2d(state.getMaze());
			}
			float max_q = calcTraingProfit(nextState.getMaze());
			float profit = R[state.getMaze()][action] + TRAINING_FACTOR * max_q;

			Q[state.getMaze()][action] = profit;

		//	if (!nextState.hasDeadlock())
				state = nextState;


		}
		//if (!isFinalGoal(state)) episodes++;
	}

}





void solve(vvc maze) {
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
		p2d(grid.getMaze());
	}
}



void printQ() {
	for (auto it : Q) {
		for (auto i : it.second) {
				cout << i.first << " " << i.second << endl;
				p2d(it.first);
		}
	}
}

void printR() {
	for (auto it : R) {
		for (auto i : it.second) {
			if (i.second > 0) {
				cout << i.second << endl;
				p2d(it.first);
			}
		}
	}
}


vvc temp =
{ {' ',' ',' ','W','W','W','W','W',' '},
  {' ','W','W','W','W','W','W','W',' '},
  {' ','W','W','W','W','W','W','W',' '},
  {' ','W','G','B','M',' ',' ','W',' '},
  {' ','W','W','W','W','W','W','W',' '},
  {' ','W','W','W','W','W','W','W','W'},
  {' ','W','W','W','W','W','W','W','W'},
  {' ','W','W','W','W','W','W','W','W'},
  {' ','W','W','W','W','W','W','W','W'} };

vvc getNextFram(vvc maze) {

	Grid grid = Grid(maze);
	string dir = "udlr";

	int maxi = INT_MIN;
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
	p2d(grid.getMaze());
	return grid.getMaze();
}


class SokoPan {

public:

	SokoPan(map < vvc, map<char, float> >& Q) {
		this->Q = Q;
	}

	void play(vvc maze) {

	}

	vvc getNextFram(vvc maze) {
		Grid grid = Grid(maze);
		string dir = "udlr";

		int maxi = -100;
		char best = 'u';
		for (char move : dir) {
			if (Q[maze][move] >= maxi) {
				best = move;
				maxi = Q[maze][move];
			}
		}

		grid = Grid(getUpdatedMaze(grid, Position::Move(best)));
		return grid.getMaze();
	}

private:
	map < vvc, map<char, float> > Q;
};

int main()
{

	MAZE = MAZE2;

	fillMazeOut(MAZE);

	fill_R();
	printR();
	run();

	vvc abd = MAZE;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		

		int yPos = 0, xPos = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				sf::Vector2i position{ xPos, yPos };

				rectangle.setPosition(sf::Vector2f(position));
				triangle.setPosition(sf::Vector2f(position));
				circle.setPosition(sf::Vector2f(position));

				

				switch (abd[i][j]) {
				case ' ':
					rectangle.setFillColor(sf::Color::Transparent);
					break;
				case 'W':
					rectangle.setFillColor(sf::Color(125, 113, 71));
					break;
				case 'G':
					rectangle.setFillColor(sf::Color::Transparent);
					circle.setFillColor(sf::Color(216, 148, 133));
					window.draw(circle);
					break;
				case 'B':
					rectangle.setFillColor(sf::Color(91, 60, 30));
					break;
				case 'Z':
					rectangle.setFillColor(sf::Color(255, 235, 0));
					break;
				case 'X':
					rectangle.setFillColor(sf::Color(0, 255, 0));
					break;
				case 'O':
					rectangle.setFillColor(sf::Color(255, 99, 71));
					break;
				case 'M':
					rectangle.setFillColor(sf::Color::Transparent);
					triangle.setFillColor(sf::Color(51, 73, 237));
					window.draw(triangle);
					break;

				}
				window.draw(rectangle);

				xPos += 20;

			}
			yPos += 20;
			xPos = 0;
		}

		window.display();

		abd = getNextFram(abd);
		
		p2d(abd);
		system("pause");
		window.clear();
	}

	
	//solve(MAZE);
	system("cls");/*
	for (int row = 0; row < abd.size(); row++)
	{
		for (int col = 0; col < abd[0].size(); col++) {
			vvc vb = MAZE;
			Grid temp = Grid(MAZE);
			vb[temp.getMan().row][temp.getMan().col] = ' ';
			if (MAZE[row][col] == EMPTY) {
				vb[temp.getMan().row][temp.getMan().col] = MAN;
				getNextFram(vb);
				cout << endl;

			}
		}
	}
	*/
	return 0;
}

