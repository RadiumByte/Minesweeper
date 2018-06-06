#include <SFML/Graphics.hpp>
#include "MineField.h"
#include <iostream>

#define SQUARE_WIDTH	32
#define SQUARE_HEIGHT   32

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT   640

#define TEXTURE_COUNT   13

#define MINE_COUNT      100
int main()
{
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "MineSweeper Game");

	MineField map(SCREEN_WIDTH / SQUARE_WIDTH, SCREEN_HEIGHT / SQUARE_HEIGHT, MINE_COUNT,
		sf::Vector2u(SQUARE_WIDTH, SQUARE_HEIGHT));
	
	sf::Texture resources[TEXTURE_COUNT];
	sf::Sprite squares[SCREEN_WIDTH / SQUARE_WIDTH * SCREEN_HEIGHT / SQUARE_HEIGHT];
	
	std::string files[] = { "empty.png", "mine.png", "flag.png", "fog.png",
							"one.png", "two.png", "three.png", "four.png",
							"five.png", "six.png", "seven.png", "eight.png", "explosion.png"};

	for (int i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (!resources[i].loadFromFile(files[i]))
			return -1;
		resources[i].setSmooth(true);
	}
	
	game_start:

	bool first_click = true;
	bool gameover = false;

	//sf::Clock timer;
	while (window.isOpen())
	{
		sf::Event event;

		// ++ Game logic
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

					if (gameover)
					{
						map.restart();
						goto game_start;
					}
						
					if (map.isSquareClosed(mousePosition))
					{
						if (first_click)
						{
							map.generate(mousePosition);
							map.prepare();
							map.openSquare(mousePosition);
							first_click = false;
						}
						else
						{
							bool mine = map.checkMine(mousePosition);
							if (mine)
							{
								map.setTile(mousePosition, MineField::TileType::EXPLOSION);
								map.unhideField();
								gameover = true;
							}
							else
							{
								map.openSquare(mousePosition);
							}
						}		
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					
					if (map.isSquareClosed(mousePosition))
						map.setVisibleTile(mousePosition, MineField::TileType::FLAG);
					else if (map.getVisibleTile(mousePosition) == MineField::TileType::FLAG)
						map.setVisibleTile(mousePosition, MineField::TileType::HIDDEN);
				}
			}
		}
		// -- Game logic

		window.clear(sf::Color::White);
		// ++ Screen draw
		//map.debugOutput();
		for (int i = 0; i < map.width*map.height; ++i)
		{
			int tileNumber = map.visibleField[i];
			squares[i].setTexture(resources[tileNumber]);

			squares[i].setPosition(sf::Vector2f(SQUARE_WIDTH * (i%map.width), SQUARE_HEIGHT * (i/map.width)));

			window.draw(squares[i]);
		}
		window.display();
		// -- Screen draw
	}

	system("pause");
	return 0;
}