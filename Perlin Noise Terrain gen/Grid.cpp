#pragma once
#include "Grid.h"

GridManager::GridManager(sf::RenderWindow* window)
{
	if (singleton.Instance()->ScreenWidth % 10 != 0 || singleton.Instance()->ScreenHeight % 10 != 0)
	{
		throw std::invalid_argument("received bad screen values");
	}

	pWindow = window;
	
	for (int j = 0; j <= singleton.Instance()->ScreenHeight; j = j + 50)
	{
		std::vector<Cell> temp;
		for (int i = 0; i <= singleton.Instance()->ScreenWidth; i = i + 50)
		{
			temp.push_back(Cell(-i, -j));
		}
		GridCells.push_back(temp);
	}


}

GridManager::~GridManager()
{
}

void GridManager::DrawGrid()
{
	for (int i = 0; i <= singleton.Instance()->ScreenWidth; i = i + 50)
	{
		for (int j = 0; j <= singleton.Instance()->ScreenHeight; j = j + 50)
		{
			pWindow->draw(GridCells[i/50][j/50].CellShape);
		}
	}
}
