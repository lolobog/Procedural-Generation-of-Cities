#pragma once
#include "Grid.h"

GridManager::GridManager(sf::RenderWindow* window)
{
	/*if (singleton.Instance()->ScreenWidth % 10 != 0 || singleton.Instance()->ScreenHeight % 10 != 0)
	{
		throw std::invalid_argument("received bad screen values");
	}*/

	pWindow = window;
	
	for (int j = 0; j <ScreenHeight; j = j + cellSize)
	{
		std::vector<Cell> temp;
		for (int i = 0; i < ScreenWidth; i = i + cellSize)
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
	for (int i = 0; i < ScreenWidth; i = i + cellSize)
	{
		for (int j = 0; j < ScreenHeight; j = j + cellSize)
		{
			pWindow->draw(GridCells[i/ cellSize][j/ cellSize].CellShape);
		}
	}
}

void GridManager::ClearGridColor()
{
	for (int i = 0; i < ScreenWidth; i = i + cellSize)
	{
		for (int j = 0; j <ScreenHeight; j = j + cellSize)
		{
			GridCells[i / cellSize][j / cellSize].CellShape.setFillColor(sf::Color::White);
		}
	}
}
