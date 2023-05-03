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

bool isPointInPoly(sf::Vector2f point, vector<sf::Vector2f> polygon) {

	int num_vertices = polygon.size();

	double x = point.x, y = point.y;

	bool inside = false;

	// Store the first point in the polygon and initialize the second point
	sf::Vector2f p1 = polygon[0], p2;

	// Loop through each edge in the polygon
	for (int i = 1; i <= num_vertices; i++) {

		// Get the next point in the polygon
		p2 = polygon[i % num_vertices];

		// Check if the point is above the minimum y coordinate of the edge
		if (y > min(p1.y, p2.y)) {

			// Check if the point is below the maximum y coordinate of the edge
			if (y <= max(p1.y, p2.y)) {

				// Check if the point is to the left of the maximum x coordinate of the edge
				if (x <= max(p1.x, p2.x)) {

					/*
						 Calculate the x-intersection of the line connecting the point to the edge
					*/
					double x_intersection = (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;

					// Check if the point is on the same line as the edge or to the left of the x-intersection
					if (p1.x == p2.x || x <= x_intersection) {

						// Flip the inside flag
						inside = !inside;
					}
				}
			}
		}

		// Store the current point as the first point for the next iteration
		p1 = p2;
	}

	// Return the value of the inside flag
	return inside;
}





Building::~Building()
{
}




void Building::construct()
{
	for (int j=0;j< buildingParts.size();j++)
	{
		if (isPointInPoly(buildingParts[j]->position, plotLimits))
		{
			sf::ConvexShape building;
			building.setPointCount(buildingParts[j]->shapePoints.size() + 1);
			for (int i = 0; i < buildingParts[j]->shapePoints.size(); i++)
			{
				building.setPoint(i, buildingParts[j]->shapePoints[i]);
			}
			building.setPoint(buildingParts[j]->shapePoints.size(), buildingParts[j]->shapePoints[0]);
			building.setFillColor(sf::Color(255, 255, 255, 255));
			//building.setOutlineColor(sf::Color::Black);
			building.setOutlineThickness(1);

			forDrawing.push_back(building);
		}
		else
		{
			delete buildingParts[j];
			buildingParts[j] = NULL;
			buildingParts.erase(buildingParts.begin() + j);
		}
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
