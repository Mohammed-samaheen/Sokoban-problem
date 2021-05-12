#pragma once
#include "Position.h"

Position::Position() {
	Position(0, 0);
}

Position::Position(int x, int y) {
	this->row = x;
	this->col = y;
}

Position::Move::Move(int mx, int my) {
	this->move_row = mx;
	this->move_col = my;
}

Position::Move::Move(char ch) {
	switch (ch) {
	case 'u': { new (this) Move(-1, 0); break; }
	case 'd': { new (this) Move(1, 0); break; }
	case 'l': { new (this) Move(0, -1); break; }
	case 'r': { new (this) Move(0, 1); break; }
	default: { new (this) Move(0, 0); }
	}
}

Position::Move Position::Move::operator+ (Move recivedMove) {
	return Move(this->move_row + recivedMove.move_row, this->move_col + recivedMove.move_col);
}



Position Position::operator+ (Move move) {
	return Position(this->row + move.move_row, this->col + move.move_col);
}

