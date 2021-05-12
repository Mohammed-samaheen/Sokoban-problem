#pragma once


#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>

#include "lip/boost/archive/text_oarchive.hpp"
#include "lip/boost/archive/text_iarchive.hpp"
#include "lip/boost/serialization/vector.hpp"
#include "lip/boost/serialization/map.hpp"

using namespace std;

typedef vector<vector<char>> vvc;

class Utils {

public:
	Utils();
	void static save(const string& filePath, map < vvc, map<char, double> >& data);
	map < vvc, map<char, double> > static load(const string& filePath);
	void static p2d(vvc v);
	void static draw(sf::RenderWindow& window, vvc maze);

};