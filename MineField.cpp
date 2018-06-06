#include "MineField.h"

MineField::MineField(unsigned int width, unsigned int height, int mines, sf::Vector2u tileSize)
{
	this->height = height;
	this->width = width;
	this->mines = mines;
	this->tileSize = tileSize;

	tileField = new int[width * height];

	visibleField = new int[width * height];
	hideField();
}
MineField::~MineField()
{
	delete[] tileField;
	delete[] visibleField;
}

void MineField::load(sf::Vector2u tileSize, const int* tiles)
{
	loadTiles(tiles);
	this->tileSize = tileSize;
}

void MineField::generate(sf::Vector2i click_position)
{
	int i = click_position.x / tileSize.x;
	int j = click_position.y / tileSize.y;
	int click_index = i + j * width;

	std::list<int> index;

	for (int i = 0; i < width * height; ++i)
		index.push_back(i);

	if (mines > width*height)
		mines = width*height - width;

	int mines_to_go = this->mines;

	std::random_device rd_pos;
	std::mt19937 mt_pos(rd_pos());
	std::uniform_int_distribution<int> dist_pos(0, width * height);

	while (mines_to_go != 0)
	{
		bool found = false;

		while (!found)
		{
			int mine_rand_pos = dist_pos(mt_pos);
			auto found_iter = std::find(index.begin(), index.end(), mine_rand_pos);
			if (found_iter != index.end())
			{
				// найден свободный индекс
				int free_index = *found_iter;
				if (free_index == click_index)
					continue;

				tileField[free_index] = MineField::TileType::MINE;
				index.erase(found_iter);
				found = true;
			}
		}
		mines_to_go--;
	}

	for (int i = 0; i < width*height; ++i)
	{
		if (tileField[i] != MineField::TileType::MINE)
			tileField[i] = MineField::TileType::EMPTY;
	}
}

void MineField::restart()
{
	hideField();

	for (int i = 0; i < width*height; ++i)
		tileField[i] = 0;
}

bool MineField::checkMine(sf::Vector2i mousePosition)
{
	if (getTile(mousePosition) == TileType::MINE)
		return true;
	return false;
}

bool MineField::checkFlag(sf::Vector2i mousePosition)
{
	if (getTile(mousePosition) == TileType::FLAG)
		return true;
	return false;
}

int MineField::getTile(sf::Vector2i mousePosition)
{
	int i = mousePosition.x / tileSize.x;
	int j = mousePosition.y / tileSize.y;
	return tileField[i + j * width];
}

int MineField::getVisibleTile(sf::Vector2i mousePosition)
{
	int i = mousePosition.x / tileSize.x;
	int j = mousePosition.y / tileSize.y;
	return visibleField[i + j * width];
}

void MineField::setTile(sf::Vector2i mousePosition, MineField::TileType type)
{
	int i = mousePosition.x / tileSize.x;
	int j = mousePosition.y / tileSize.y;
	tileField[i + j * width] = type;
}

void MineField::setVisibleTile(sf::Vector2i mousePosition, MineField::TileType type)
{
	int i = mousePosition.x / tileSize.x;
	int j = mousePosition.y / tileSize.y;
	visibleField[i + j * width] = type;
}

void MineField::loadTiles(const int* from)
{
	for (int i = 0; i < height * width; ++i)
		this->tileField[i] = from[i];
}

void MineField::debugOutput()
{
	for (int i = 0; i < height * width; ++i)
	{
		if (i % width == 0)
			std::cout << std::endl;
		std::cout << tileField[i] << " ";
	}
	std::cout << std::endl;
}

void MineField::hideField()
{
	for (int i = 0; i < height * width; ++i)
		visibleField[i] = MineField::TileType::HIDDEN;
}

bool MineField::isSquareClosed(sf::Vector2i mousePosition)
{
	if (this->getVisibleTile(mousePosition) == MineField::TileType::HIDDEN)
		return true;
	return false;
}

void MineField::openSquare(sf::Vector2i mousePosition)
{
	this->setVisibleTile(mousePosition, (MineField::TileType)this->getTile(mousePosition));
}

void MineField::unhideField()
{
	for (int i = 0; i < height * width; ++i)
		visibleField[i] = tileField[i];
}

int MineField::checkSurround(int index)
{
	int count = 0;
	if (index == 0)
	{
		count += (int) tileField[index + 1];           //справа
		count += (int) tileField[index + width];       //снизу
		count += (int) tileField[index + width + 1];   //справа-снизу
	}
	else if (index == width - 1)
	{
		count += (int) tileField[index - 1];          //слева
		count += (int) tileField[index + width];       //снизу
		count += (int) tileField[index + width - 1];   //слева-снизу
	}
	else if (index == width*height - width)
	{
		count += (int)tileField[index + 1];          //справа
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width + 1];   //справа-сверху
	}
	else if (index == width*height - 1)
	{
		count += (int)tileField[index - 1];       //слева
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width - 1];   //слева-сверху
	}
	else if (index / width == 0)
	{
		count += (int)tileField[index - 1];       //слева
		count += (int)tileField[index + 1];       //справа
		count += (int)tileField[index + width];       //снизу
		count += (int)tileField[index + width - 1];   //слева-снизу
		count += (int)tileField[index + width + 1];   //справа-снизу
	}
	else if (index % width == 0)
	{
		count += (int)tileField[index + 1];       //справа
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width + 1];   //справа-сверху
		count += (int)tileField[index + width + 1];   //справа-снизу
		count += (int)tileField[index + width];       //снизу
	}
	else if (index / width == width - 1)
	{
		count += (int)tileField[index - 1];       //слева
		count += (int)tileField[index + 1];       //справа
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width - 1];   //слева-сверху
		count += (int)tileField[index - width + 1];   //справа-сверху
	}
	else if (index % width == width - 1)
	{
		count += (int)tileField[index - 1];       //слева
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width - 1];   //слева-сверху
		count += (int)tileField[index + width];       //снизу
		count += (int)tileField[index + width - 1];   //слева-снизу
	}
	else
	{
		count += (int)tileField[index - 1];       //слева
		count += (int)tileField[index + 1];       //справа
		count += (int)tileField[index - width];       //сверху
		count += (int)tileField[index - width + 1];   //справа-сверху
		count += (int)tileField[index - width - 1];   //слева-сверху
		count += (int)tileField[index + width];       //снизу
		count += (int)tileField[index + width - 1];   //слева-снизу
		count += (int)tileField[index + width + 1];   //справа-снизу
	}
	return count;
}

void MineField::prepare()
{
	int* temp_field = new int[width*height];

	for (int i = 0; i < width*height; ++i)
		temp_field[i] = tileField[i];

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			int position = j + i * width;
			MineField::TileType tileNumber = (MineField::TileType) tileField[position];

			if (tileNumber == MineField::TileType::EMPTY)
			{
				int count = checkSurround(position);
				if (count == 1)
					temp_field[position] = (int)MineField::TileType::ONE;
				else if (count == 2)
					temp_field[position] = (int)MineField::TileType::TWO;
				else if (count == 3)
					temp_field[position] = (int)MineField::TileType::THREE;
				else if (count == 4)
					temp_field[position] = (int)MineField::TileType::FOUR;
				else if (count == 5)
					temp_field[position] = (int)MineField::TileType::FIVE;
				else if (count == 6)
					temp_field[position] = (int)MineField::TileType::SIX;
				else if (count == 7)
					temp_field[position] = (int)MineField::TileType::SEVEN;
				else if (count == 8)
					temp_field[position] = (int)MineField::TileType::EIGHT;
			}
		}
	}
	for (int i = 0; i < width*height; ++i)
		tileField[i] = temp_field[i];
	delete[] temp_field;
}