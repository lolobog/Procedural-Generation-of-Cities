#pragma once

#include <SFML/Graphics.hpp>
#include "Variables.h"
#include <iostream>




class Cell
{

private:
	sf::Vector2f ScreenPosition;
	
public:
	sf::RectangleShape CellShape;
	float cellSize = 5;

	Cell(float xPos, float yPos) 
	{ 
		ScreenPosition=sf::Vector2f(xPos, yPos); 
		CellShape.setSize(sf::Vector2f(cellSize, cellSize));
		CellShape.setOutlineThickness(1.0f);
		CellShape.setOutlineColor(sf::Color::Black);
		CellShape.setOrigin(ScreenPosition);
	}
	Cell() {};
	~Cell() {};

	void SetCellPosition(float x,float y)
	{
		ScreenPosition = sf::Vector2f(x, y);
		CellShape.setOrigin(ScreenPosition);
	}

	void SetCellSize(float x, float y)
	{
		CellShape.setSize(sf::Vector2f(x, y));
	}
	

	
	
	
};

class GridManager:public Cell
{
	

public:
	GridManager(sf::RenderWindow* window);
	~GridManager();

	std::vector<std::vector<Cell>> GridCells;

	void DrawGrid();

	void ClearGridColor();
	

private:
	Singleton singleton;
	sf::RenderWindow* pWindow;

};