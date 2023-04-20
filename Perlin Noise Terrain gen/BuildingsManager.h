#pragma once
#include "RoadManager.h"

sf::Vector2f findCenter(vector<sf::Vector2f> points);

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
			break;
		}

		case 'R':
		{

			break;
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
	int wallSize ;
public:
	Building(sf::Vector2f center, vector<sf::Vector2f> limits,int iterations)
	{
		wallSize = findWallSize(limits);
		//buildingParts.push_back(new BuildingNode(center, 'C', wallSize));
		buildingParts.push_back(new BuildingNode(NULL,sf::Vector2f(center.x+wallSize/2,center.y+wallSize/2), 'C','A', wallSize));
		divide(buildingParts[0], buildingParts);
		buildingParts.erase(buildingParts.begin());
		iterate(iterations);
		construct();
	}
	
	~Building();

	vector<sf::ConvexShape>forDrawing;
	void construct();
	void applyRules();
	void iterate(int numberOfIterations);
	float findWallSize(vector<sf::Vector2f>limits)
	{
		if (limits.size() == 3)
		{
			double a = sqrt(pow(limits[1].x - limits[0].x, 2) + pow(limits[1].y - limits[0].y, 2));
			double b = sqrt(pow(limits[2].x - limits[1].x, 2) + pow(limits[2].y - limits[1].y, 2));
			double c = sqrt(pow(limits[0].x - limits[2].x, 2) + pow(limits[0].y - limits[2].y, 2));
			double s = (a + b + c) / 2;
			double area = sqrt(s * (s - a) * (s - b) * (s - c));
			double radius = area / s;
			return radius/2.5;
		}
		else
		{
			return distanceF(limits[0].x, limits[0].y, limits[1].x, limits[1].y)/5;
		}
	};

	void shrink(BuildingNode* node, vector<BuildingNode*> &newNodes);
	void divide(BuildingNode* node, vector<BuildingNode*> &newNodes);
	
	

	


};

class City
{
public:

	City(RoadManager* roadM,int iterations);
	
	
	~City();

	void extractPlotLimits();
	void findPlotCenters();
	void drawCenters();
	void create();
	void display();



private:
	int numberOfIterations;
	RoadManager* roadManager;
	vector<Building*> buildings;
	vector<vector<sf::Vector2f>> plotLimits;
	vector<sf::Vector2f> plotCenters;
	

};