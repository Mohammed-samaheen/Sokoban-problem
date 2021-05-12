#pragma once

#include "Utils.h"

Utils::Utils() {
	return;
}

void  Utils::save(const string& filePath, map < vector<vector<char>>, map<char, double> >& data) {
	ofstream ofs(filePath);
	boost::archive::text_oarchive oa(ofs);

	oa << data;
}

map < vector<vector<char>>, map<char, double> > Utils::load(const string& filePath) {
	map < vector<vector<char>>, map<char, double> > data;
	ifstream ifs(filePath);
	boost::archive::text_iarchive ia(ifs);
	ia >> data;
	return data;

}

void Utils::draw(sf::RenderWindow& window, vector<vector<char>> maze) {
	sf::RectangleShape rectangle(sf::Vector2f(19, 19));
	sf::CircleShape circle(8);
	sf::CircleShape triangle(10, 6);

	window.clear();

	int yPos = 0, xPos = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			sf::Vector2i position{ xPos, yPos };

			rectangle.setPosition(sf::Vector2f(position));
			triangle.setPosition(sf::Vector2f(position));
			circle.setPosition(sf::Vector2f(position));

			switch (maze[i][j]) {
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

}

void Utils::p2d(vector<vector<char>> v) {
	for (auto it : v) {
		for (auto a : it) {
			cout << a << " ";
		}
		cout << endl;
	}
	cout << endl;
}