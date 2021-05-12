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
#include "SokoPan.h"

using namespace std;
using namespace LibConstants;

#define vvc vector<vector<char>>

sf::RenderWindow window(sf::VideoMode(500, 500), "Sokoban");


vvc MAZE =
{ {' ',' ',' ','W','W','W','W','W',' '},
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
{ {'W','W','W','W','W','W','W','W','W'},
 {'W','W','W','W','W','W','W','W','W'},
 {'W','W','W','W','W','W','W','W','W'},
 {'W','W',' ',' ',' ','W','M',' ','W'},
 {'W','W',' ',' ',' ',' ','B',' ','W'},
 {'W','W',' ',' ',' ','W',' ',' ','W'},
 {'W','W','W','W','W','W','B','G','W'},
 {'W','W','W','W','W','W',' ','G','W'},
 {'W','W','W','W','W','W','W','W','W'} };
int main()
{

	SokoPan sokoPan = SokoPan(MAZE4);

	string fileName = "Text3.txt";

	map < vvc, map<char, double> > Q;

	Q = sokoPan.train(window, true);
	Utils::save(fileName, Q);

	Q = Utils::load(fileName);

	int a, b;
	while (cin >> a >> b) {
		Utils::p2d(MAZE);
		Grid g = Grid(MAZE);
		vvc temp = MAZE;
		__Cell(temp, g.getMan()) = EMPTY;
		__Cell(temp, Position(a, b)) = MAN;
		Grid r = Grid(temp);
		while (!sokoPan.isFinalGoal(r)) {
			temp = SokoPan::getNextFram(Q, window, temp);
			r = Grid(temp);
			system("pause");
		}
	}

	window.close();

	return 0;
}

