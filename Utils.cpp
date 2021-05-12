#pragma once

#include <bits/stdc++.h>
#include <fstream>
#include <map>

#include "lip/boost/archive/text_oarchive.hpp"
#include "lip/boost/archive/text_iarchive.hpp"
#include "lip/boost/serialization/vector.hpp"
#include "lip/boost/serialization/map.hpp"

using namespace std;

void save(const string& filePath, map <vector<vector<char>>, map <char, int>>& data);
map <vector<vector<char>>, map <char, int>> load(const string& filePath);

class Utils {

	void static save(const string& filePath, map <vector<vector<char>>, map <char, int>>& data) {
		ofstream ofs(filePath);
		boost::archive::text_oarchive oa(ofs);

		oa << data;
	}

	map <vector<vector<char>>, map <char, int>> static load(const string& filePath) {
		map <vector<vector<char>>, map <char, int>> data;
		ifstream ifs(filePath);
		boost::archive::text_iarchive ia(ifs);
		ia >> data;
		return data;

	}

};