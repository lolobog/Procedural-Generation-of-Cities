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

	Cell(float xPos, float yPos) 
	{ 
		ScreenPosition=sf::Vector2f(xPos, yPos); 
		CellShape.setSize(sf::Vector2f(50.0f, 50.0f));
		CellShape.setOutlineThickness(1.0f);
		CellShape.setOutlineColor(sf::Color::Black);
		CellShape.setOrigin(ScreenPosition);
	}
	~Cell() {};

	void SetCellPosition(float x,float y)
	{
		ScreenPosition = sf::Vector2f(x, y);
	}

	void SetCellSize(float x, float y)
	{
		CellShape.setSize(sf::Vector2f(x, y));
	}
	

	
	
	
};

class GridManager
{


public:
	GridManager(sf::RenderWindow* window);
	~GridManager();

	std::vector<std::vector<Cell>> GridCells;

	void DrawGrid();

	

private:
	Singleton singleton;
	sf::RenderWindow* pWindow;

};