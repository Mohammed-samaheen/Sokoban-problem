#pragma once

#include "Utils.h"

Utils::Utils() {
	return;
}

void  Utils::save(const string& filePath, map < vvc, map<char, float> >& data) {
	ofstream ofs(filePath);
	boost::archive::text_oarchive oa(ofs);

	oa << data;
}

map < vvc, map<char, float> > Utils::load(const string& filePath) {
	map < vvc, map<char, float> > data;
	ifstream ifs(filePath);
	boost::archive::text_iarchive ia(ifs);
	ia >> data;
	return data;

}
