#pragma once
#include "RoadManager.h"

sf::Vector2f findCenter(vector<sf::Vector2f> points);
class Roof
{

};

class Wall
{

};

class Floor
{

};

class BuildingNode
{
public:
	BuildingNode(BuildingNode* _predecessor, sf::Vector2f center, char shape,char _symbol,int _wallSize)
	{
		predecessor = _predecessor;
		position = center;
		wallSize = _wallSize;
		symbol = _symbol;

		switch (shape)
		{
		case 'C':
		{
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y - wallSize));
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y - wallSize));
		}

		default:
		{
			break;
		}
		}
		
	}

	BuildingNode(BuildingNode* _predecessor, char shape, char _symbol, int _wallSize)
	{
		predecessor = _predecessor;
		position = predecessor->position;
		wallSize = _wallSize;
		symbol = _symbol;

		switch (shape)
		{
		case 'C':
		{
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y - wallSize));
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y - wallSize));
		}

		default:
		{
			break;
		}
		}

	}

	BuildingNode(sf::Vector2f center, char shape, int wallSize)
	{
		position = center;

		switch (shape)
		{
		case 'C':
		{
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y + wallSize));
			shapePoints.push_back(sf::Vector2f(position.x - wallSize, position.y - wallSize));
			shapePoints.push_back(sf::Vector2f(position.x + wallSize, position.y - wallSize));
		}

		default:
		{
			break;
		}
		}
	}

	BuildingNode* predecessor=NULL;


	sf::Vector2f position;
	vector<sf::Vector2f>shapePoints;
	int wallSize;
	char symbol;

};

class Building
{
	string axiom;
	vector<BuildingNode*>buildingParts;
	int wallSize = random(5,10);
public:
	Building(sf::Vector2f center, vector<sf::Vector2f> limits)
	{
		buildingParts.push_back(new BuildingNode(center, 'C', wallSize));
		divide(buildingParts[0], buildingParts);
		buildingParts.erase(buildingParts.begin());
		iterate(3);
		construct();
	}
	
	~Building();

	vector<sf::ConvexShape>forDrawing;
	void construct();
	void applyRules();
	void iterate(int numberOfIterations);

	void shrink(BuildingNode* node, vector<BuildingNode*> &newNodes);
	void divide(BuildingNode* node, vector<BuildingNode*> &newNodes);
	
	

	


};

class City
{
public:

	City(RoadManager* roadM);
	
	
	~City();

	void extractPlotLimits();
	void findPlotCenters();
	void drawCenters();
	void create();
	void display();



private:
	RoadManager* roadManager;
	vector<Building*> buildings;
	vector<vector<sf::Vector2f>> plotLimits;
	vector<sf::Vector2f> plotCenters;
	

};