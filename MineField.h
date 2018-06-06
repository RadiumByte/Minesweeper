#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

class MineField
{
public:
	enum TileType
	{
		EMPTY, MINE, FLAG, HIDDEN, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, EXPLOSION
	};

	MineField(unsigned int width, unsigned int height, int mines, sf::Vector2u tileSize);
	~MineField();

	// загрузка поля из массива
	void load(sf::Vector2u tileSize, const int* tiles);

	// генерация случайного поля
	void generate(sf::Vector2i click_position);

	bool checkMine(sf::Vector2i mousePosition);
	bool checkFlag(sf::Vector2i mousePosition);

	bool isSquareClosed(sf::Vector2i mousePosition);

	void hideField();
	void unhideField();
	void restart();

	void prepare();
	int checkSurround(int index);

	unsigned int width;
	unsigned int height;
	int mines;

	void openSquare(sf::Vector2i mousePosition);

	int getTile(sf::Vector2i mousePosition);
	int getVisibleTile(sf::Vector2i mousePosition);

	void setTile(sf::Vector2i mousePosition, TileType type);
	void setVisibleTile(sf::Vector2i mousePosition, TileType type);

	void debugOutput();

	int* visibleField;

private:

	void loadTiles(const int* from);

	int* tileField;

	sf::Vector2u tileSize;
};
