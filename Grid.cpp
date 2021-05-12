#pragma once

#include "Grid.h"

Grid::Grid(int size) {
	new (this) Grid(vvc(size, vector<char>(size, ' ')));
}

Grid::Grid() {
	new (this) Grid({});
}

Grid::Grid(vvc maze) {
	this->maze = maze;
	getManPosition();
}

vvc Grid::getMaze() {
	return this->maze;
}

bool Grid::operator==(vvc& recivedMaze) {

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

bool Grid::isState() {
	return this->maze.size() > 0 && this->maze[0].size() > 0;
}

bool Grid::hasDeadlock() {
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

Position Grid::getMan() {
	return man;
}

bool Grid::isOutBorder(Position pos) {
	return isOutBorder(pos.row, pos.col);
}

bool Grid::isOutBorder(int row, int col) {
	return (row < 0 || col < 0 || row >= maze.size() || maze.size() == 0 || col >= maze[0].size());
}

bool Grid::isTypeCell(vvc state, Position pos, char type) {

	if (Grid(state).isOutBorder(pos)) {
		return false;
	}

	return  __Cell(state, pos) == type;
}

bool Grid::isTypeCell(vvc state, Position pos, vector<char> types) {
	for (char type : types) {
		if (isTypeCell(state, pos, type)) {
			return true;
		}
	}
	return false;
}

bool Grid::isDeadlock(int row, int col) {

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

void Grid::getManPosition() {

	this->man = Position(-1, -1);

	for (int row = 0; row < this->maze.size(); row++) {
		for (int col = 0; col < this->maze[row].size(); col++) {
			if (this->maze[row][col] == MAN || this->maze[row][col] == MAN_GOAL) {
				this->man = Position(row, col);
			}
		}
	}
}
