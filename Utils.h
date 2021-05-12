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


class Utils {

public:
	Utils();
	void static save(const string& filePath, map < vector<vector<char>>, map<char, double> >& data);
	map < vector<vector<char>>, map<char, double> > static load(const string& filePath);
	void static p2d(vector<vector<char>> v);
	void static draw(sf::RenderWindow& window, vector<vector<char>> maze);

};