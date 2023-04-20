#include "BuildingsManager.h"

City::City(RoadManager* roadM, int iterations)
{
	roadManager = roadM;
	numberOfIterations = iterations;
}

City::~City()
{
}

void City::extractPlotLimits()
{
	if (roadManager!=NULL&& roadManager->plots.size() > 0)
	{
		for (auto plot : roadManager->plots)
		{
			vector<sf::Vector2f> tempPoints;
			for (auto point : plot)
			{
				tempPoints.push_back(point->endPos);
			}

			plotLimits.push_back(tempPoints);
		}
	}
}

void City::findPlotCenters()
{
	if (plotCenters.size() > 0)
	{
		plotCenters.clear();
	}
	for (auto plot : plotLimits)
	{
		plotCenters.push_back(findCenter(plot));
	}

}

void City::drawCenters()
{
	if (this != NULL)
	{
		if (plotCenters.size() > 0)
		{
			for (auto center : plotCenters)
			{
				sf::CircleShape point(2);
				point.setPosition(sf::Vector2f(center.x - point.getRadius(), center.y - point.getRadius()));
				point.setFillColor(sf::Color::Black);
				roadManager->refWindow->draw(point);
			}
		}
	}
}

void City::create()
{
	for (int i=0;i< plotCenters.size();i++)
	{
		buildings.push_back(new Building(plotCenters[i], plotLimits[i],numberOfIterations));
	}
}

void City::display()
{
	if (this != NULL)
	{
		if (plotCenters.size() > 0)
		{
			for (auto element : buildings)
			{
				for (auto subElement : element->forDrawing)
				{
					roadManager->refWindow->draw(subElement);
				}
			}
		}
	}
}

sf::Vector2f findCenter(vector<sf::Vector2f> points)
{
	sf::Vector2f center;
	center.x = 0.0;
	center.y = 0.0;
	for (int i = 0; i < points.size(); i++) {
		center.x += points[i].x;
		center.y += points[i].y;
	}
	center.x /= points.size();
	center.y /= points.size();
	return center;
}







Building::~Building()
{
}

void Building::construct()
{
	for (int j=0;j< buildingParts.size();j++)
	{
		sf::ConvexShape building;
		building.setPointCount(buildingParts[j]->shapePoints.size()+1);
		for (int i = 0; i < buildingParts[j]->shapePoints.size();i++)
		{
			building.setPoint(i, buildingParts[j]->shapePoints[i]);
		}
		building.setPoint(buildingParts[j]->shapePoints.size(), buildingParts[j]->shapePoints[0]);
		building.setFillColor(sf::Color(255, 255, 255, 255));
		//building.setOutlineColor(sf::Color::Black);
		building.setOutlineThickness(1);
	
		forDrawing.push_back(building);
	}
}

void Building::applyRules()
{
	vector<BuildingNode*> tempParts;
	for (auto &element : buildingParts)
	{
		switch (element->symbol)
		{
		case 'A': //Room
		{
			int chance = random(1, 6);
			if (chance %2==0)
			{
				divide(element,tempParts);
			}
			else
				if (chance == 1)
				{
					shrink(element,tempParts);
				}
				else
				{
					tempParts.push_back(new BuildingNode(element, 'C', 'B', element->wallSize));
				}
			

			
			break;
		}

		case 'B': //Floor Terminal Room
		{
			tempParts.push_back(new BuildingNode(element, 'C', 'B', element->wallSize));
			break;
		}


	

		default:
		{
			break;
		}
		}
	}

	buildingParts = tempParts;
}

void Building::iterate(int numberOfIterations)
{
	int i = 0;
	while (i < numberOfIterations)
	{
		applyRules();
		i++;
	}
}

void Building::shrink(BuildingNode* node, vector<BuildingNode*> &newNodes)
{
	sf::Vector2f newPos;
	
	int newWallsSize = node->wallSize ;
	if (node->predecessor != NULL)
	{
		if (node->position.x < node->predecessor->position.x)
		{
			newPos.x += newWallsSize;
		}
		else
		{
			newPos.x -= newWallsSize;
		}

		if (node->position.y < node->position.y)
		{
			newPos.y += newWallsSize;
		}
		else
		{
			newPos.y -= newWallsSize;
		}
	}
	else
	{
		newPos = node->position;
	}
	if (random(1, 2) == 1)
		newNodes.push_back(new BuildingNode(node, newPos, 'C', 'A', newWallsSize));
	else
		newNodes.push_back(new BuildingNode(node, newPos, 'C', 'B', newWallsSize));
}

void Building::divide(BuildingNode* node, vector<BuildingNode*> &newNodes)
{
	newNodes.push_back(new BuildingNode(node,sf::Vector2f(node->position.x+wallSize/2, node->position.y + wallSize / 2), 'C','A', node->wallSize / 2));
	newNodes.push_back(new BuildingNode(node, sf::Vector2f(node->position.x - wallSize / 2, node->position.y + wallSize / 2), 'C', 'A', node->wallSize / 2));
	newNodes.push_back(new BuildingNode(node, sf::Vector2f(node->position.x - wallSize / 2, node->position.y - wallSize / 2), 'C', 'A', node->wallSize / 2));
	newNodes.push_back(new BuildingNode(node, sf::Vector2f(node->position.x + wallSize / 2, node->position.y - wallSize / 2), 'C', 'A', node->wallSize / 2));

}
