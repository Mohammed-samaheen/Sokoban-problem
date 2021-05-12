#pragma once

#include <bits/stdc++.h>
#include <fstream>
#include <map>

#include "lip/boost/archive/text_oarchive.hpp"
#include "lip/boost/archive/text_iarchive.hpp"
#include "lip/boost/serialization/vector.hpp"
#include "lip/boost/serialization/map.hpp"

using namespace std;

typedef vector<vector<char>> vvc;
#define float long double

class Utils {

public:
	Utils();
	void static save(const string& filePath, map < vvc, map<char, float> >& data);
	map < vvc, map<char, float> > static load(const string& filePath);

};