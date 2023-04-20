#include "RoadManager.h"

RoadManager::RoadManager(int chunkID,sf::RenderWindow* window,int* roadsType, PerlinNoise2D* pn)
{
	perlinRef = pn;
	variables = { 'C' ,'F','R','L','B'};
	output = { 'C' };
	RoadNetwork->setRoot(findChunkCenter(chunkID),'C');
	currentChunk = chunkID;
	refWindow = window;
	typeOfRoads = roadsType;
	
	
}

RoadManager::~RoadManager()
{
}


int random(int low, int high)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);

}


bool RoadManager::checkRules(sf::Vector2f pos, sf::Vector2f parent,bool applyRules)
{
	if (applyRules == true)
	{
		if (*typeOfRoads == 1)
		{
			if (
				isInChunkBounds(pos, currentChunk) &&
				isInUndesireableTerrain(pos) == false

				)
			{
				cout << pos.x << " " << pos.y << " is  respecting the rules\n";
				return true;
			}
			else
			{
				cout << pos.x << " " << pos.y << " is  NOT respecting the rules\n";
				return false;
			}
		}
		else
		{
			if (
				isInChunkBounds(pos, currentChunk) &&
				isInUndesireableTerrain(pos) == false 

				)
			{
				cout << pos.x << " " << pos.y << " is  respecting the rules\n";
				return true;
			}
			else
			{
				cout << pos.x << " " << pos.y << " is  NOT respecting the rules\n";
				return false;
			}
		}
	}
	else
		return true;
}


void RoadManager::GenerateRoadChunk(Node* element, float plotGenerationChance, char dir, float length, bool applyRules)
{
	sf::Vector2f pos;

	bool doNotGenerate = false;

	if (*typeOfRoads == 1)
	{
		if (dir == 'R' || dir == 'L')
			pos = sf::Vector2f(element->endPos.x + length, element->endPos.y);
		else
			pos = sf::Vector2f(element->endPos.x, element->endPos.y + length);
	}
	else
	{

		if (random(1, 2) == 1)
		{
			if (dir == 'R' || dir == 'L')
				pos = sf::Vector2f(element->endPos.x + length, element->endPos.y + random(0, 10));
			else
				pos = sf::Vector2f(element->endPos.x + random(0, 10), element->endPos.y + length);
		}
		else
		{
			if (dir == 'R' || dir == 'L')
			pos = sf::Vector2f(element->endPos.x + length, element->endPos.y);
		else
			pos = sf::Vector2f(element->endPos.x, element->endPos.y + length);
		}

		auto nearbyNode = RoadNetwork->findNearbyNode(pos, 19);

		if (nearbyNode != nullptr)
		{
			sf::Vector2f tempNode = nearbyNode->endPos;
			if (isInUndesireableTerrain(sf::Vector2f((tempNode.x + element->endPos.x) / 2, (tempNode.y + element->endPos.y) / 2)) == false && RoadNetwork->isIntersecting(pos,element->endPos)==false)
			{
				pos = tempNode;
			}
			else
				doNotGenerate = true;
		}
	}

	if (doNotGenerate == false)
	{
		if (checkRules(pos, element->endPos, applyRules))
		{
			if (RoadNetwork->isOverlappingNode(pos) == false)
			{
				RoadNetwork->newLink(element, pos, dir);
			}
			else
			{
				RoadNetwork->connectNodes(RoadNetwork->getNode(pos), element);
			}
		
		}
	}
}

void RoadManager::applyRules(int iterations)
{
	float generationBias = 100;
	float plotGenerationChance = 10;
	int i = 0;
	
	sf::Clock timer;
	

	while (iterations > 0&&timer.getElapsedTime().asSeconds()<60)
	{
	

		
		for (auto element : RoadNetwork->CurrentNodes)
		{
			
		

			if (element->rulesApplied == false)
			{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// STRAIGHT ROADS /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (*typeOfRoads == 1)
				{
					switch (element->nodeType)
					{

					case 'C':
					{
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength,false);
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, false);
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, false);
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, false);

						break;
					}
					case 'F':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength,true);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);

						break;
					}
					case 'R':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, true);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

						break;
					}
					case 'L':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);

						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, true);

						break;
					}
					case 'B':
					{

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);

						break;
					}



					default:
						break;
					}
				}
				else
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// MIXED ROADS /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if (*typeOfRoads == 2)
					{
						switch (element->nodeType)
						{

						case 'C':
						{
							GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, false);
							GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, false);
							GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, false);
							GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, false);

							break;
						}
						case 'F':
						{
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, true);
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);

							break;
						}
						case 'R':
						{
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, true);
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

							break;
						}
						case 'L':
						{
							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);

							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength, true);

							break;
						}
						case 'B':
						{

							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength, true);

							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength, true);

							if (random(1, 100) <= generationBias)
								GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength, true);

							break;
						}



						default:
							break;
						}
					}
			}
			
				generationBias -= 1;
			element->rulesApplied = true;
		}
	
		RoadNetwork->CurrentNodes = RoadNetwork->IterationNodes;
		RoadNetwork->IterationNodes.clear();
		iterations--;
		generatePlots(RoadNetwork->AllNodes, NULL, NULL, 0);
	}

	
}



void RoadManager::drawRoads()
{
	vector<Node*> usedNodes;
	int numberOfRoads = 0;

	if (this != nullptr)
	{
		if (perlinRef != nullptr)
		{
			for (auto element : RoadNetwork->AllNodes)
			{
				if (showNodes == true)
				{
					sf::CircleShape circle(4);
					circle.setPosition(sf::Vector2f(element->endPos.x - circle.getRadius(), element->endPos.y - circle.getRadius()));
					circle.setFillColor(sf::Color(0, 0, 0, 64));
					refWindow->draw(circle);
				}
				for (auto connection : element->nodeLinks)
				{
					if (usedNodes.size() >= 0)
					{
						if (std::find(usedNodes.begin(), usedNodes.end(), connection) == usedNodes.end()) //If connection already exists
						{
							sf::Vertex line[] =
							{
								sf::Vertex(element->endPos),
								sf::Vertex(connection->endPos)
							};
							line->color = sf::Color(255, 255, 255, 255);
							refWindow->draw(line, 2, sf::Lines);
							numberOfRoads++;
						}
					}


					if (element->parent != nullptr)
					{
						if (std::find(usedNodes.begin(), usedNodes.end(), element->parent) == usedNodes.end())
						{
							sf::Vertex line[] =
							{
								sf::Vertex(element->parent->endPos),
								sf::Vertex(element->endPos)
							};
							refWindow->draw(line, 2, sf::Lines);
							numberOfRoads++;
						}
					}
				}

				usedNodes.push_back(element);

			

			}

			

			if (showPlots == true)
			{
				for (auto plot : plots)
				{

					sf::ConvexShape plotOutline;
					plotOutline.setPointCount(plot.size() + 1);

					for (int i = 0; i < plot.size(); i++)
					{
						plotOutline.setPoint(i, plot[i]->endPos);
					}
					plotOutline.setPoint(plot.size(), plot[0]->endPos);
					plotOutline.setFillColor(sf::Color(255, 255, 255, 30));
					plotOutline.setOutlineColor(sf::Color(255, 0, 0, 255));
					plotOutline.setOutlineThickness(2);

					refWindow->draw(plotOutline);

				}
			}
			/*for (auto element : plots)
			{

				sf::RectangleShape rectangle(sf::Vector2f(roadLength / 2, roadLength / 2));
				rectangle.setPosition(element);

				refWindow->draw(rectangle);
			}*/
		}
	}

	//cout << numberOfRoads<<'\n';

	
}


bool RoadManager::plotExsists(vector<Node*> plot)
{
	
	for (auto element : plots)
	{
		int matchingPlots = 0;
		for (auto x : plot)
		{
		
			for (auto y : element)
			{
				if (y == x)
				{
					matchingPlots++;
				}
			}
		}

		if (matchingPlots == plot.size())
		{
			
			return true;
		}
	}
	
	return false;
}

bool RoadManager::isInTempPlot(Node* node)
{
	for (auto element : tempPlot)
	{
		if (node == element)
			return true;
	}
	return false;
}

bool RoadManager::isSubPlot(vector<Node*> plot, vector<vector<Node*>> plots)
{
	
		for (auto& element : plots)
		{
			int sharedNodes = 0;
			for (auto& node : element)
			{
				for (auto& plotNode : plot)
				{
					if (node == plotNode)
					{
						sharedNodes++;
					}
				}
			}
			if (sharedNodes >=3)
			{
				return true;
			}
		}

		return false;
	
}

void RoadManager::generatePlots(std::vector<Node*>AllNodes, Node* previousNode, Node* targetNode, int depth)
{
	int i = 0;
	while (i < AllNodes.size())
	{
		Node* element = AllNodes[i];
	
		
		if (isInTempPlot(element) == false||(element==targetNode&&tempPlot.size()>2 && element != tempPlot[tempPlot.size() - 3]))
		{
			if (depth < 4)
			{
				bool nodeFound = false;
				tempPlot.push_back(element);

				if (targetNode != previousNode )
				{
					if (element->parent == targetNode)
					{
						nodeFound = true;
						if (plotExsists(tempPlot) == false&&isSubPlot(tempPlot,plots)==false)
						{
							plots.push_back(tempPlot);
						}
						tempPlot.pop_back();
						

					}

					for (auto subELement : element->nodeLinks)
					{
						if (subELement == targetNode)
						{
							nodeFound = true;
							if (plotExsists(tempPlot) == false)
							{
								plots.push_back(tempPlot);
							}
							tempPlot.pop_back();
							break;

						}
					}
				}

				if (targetNode == NULL)
				{
					targetNode = element;
				}

				


				if (nodeFound == false)
				{
					if (element!=NULL)
					{
						generatePlots(element->nodeLinks, element, targetNode, depth + 1);
						
					}
					if (tempPlot.size() > 0)
					{
						tempPlot.pop_back();
					}
					if (depth == 0)
						targetNode = NULL;
				}
			}

		}

		//if (depth == 0)
	/*	{
			for (auto plot : allPlots)
			{
				if (plot.size() < chosenPlotSize || chosenPlotSize == 0)
				{
					chosenPlotSize = plot.size();
					chosenPlot = plot;
				}
			}
			if (chosenPlot.size() > 0)
			{
				plots.push_back(chosenPlot);
				allPlots.clear();
				chosenPlot.clear();
				chosenPlotSize = 0;
			}
		}*/
		
		i++;

	}


}




