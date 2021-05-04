#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "Sokoban");
    sf::RectangleShape rectangle(sf::Vector2f(20, 20));
    vector<vector<char>> MAZE = { {' ',' ',' ','W','W','W','W','W',' '},
                                 {' ','W','W','W',' ',' ',' ','W',' '},
                                 {' ','W','G',' ','B',' ',' ','W',' '},
                                 {' ','W','W','W',' ','B','G','W',' '},
                                 {' ','W','G','W','W','B',' ','W',' '},
                                 {' ','W',' ','W',' ','G',' ','W','W'},
                                 {' ','W','B','B','G','B','B','G','W'},
                                 {' ','W',' ',' ',' ','G',' ',' ','W'},
                                 {' ','W','W','W','W','W','W','W','W'} };

    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        int yPos = 0, xPos = 0;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                sf::Vector2i position{ xPos, yPos };
                rectangle.setPosition(sf::Vector2f(position));
                switch (MAZE[i][j]) {
                case ' ':
                    rectangle.setFillColor(sf::Color::Transparent);
                    break;
                case 'W':
                    rectangle.setFillColor(sf::Color(125, 113, 71));
                    break;
                case 'G':
                    rectangle.setFillColor(sf::Color(216, 148, 133));
                    break;
                case 'B':
                    rectangle.setFillColor(sf::Color(238, 184, 100));
                    break;
                case 'M':
                    rectangle.setFillColor(sf::Color(43, 150, 237));
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

    return 0;
}
