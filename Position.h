#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

class Position {

public:

	int row;
	int col;

	Position();

	Position(int x, int y);

	struct Move {

	public:
		Move(int mx, int my);

		Move(char ch);

		Move operator+ (Move recivedMove);

		int move_row;
		int move_col;
	};

	Position operator+ (Move move);

};
