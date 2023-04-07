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


int RoadManager::random(int low, int high)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);

}


bool RoadManager::checkRules(sf::Vector2f pos)
{
	if (
		isInChunkBounds(pos, currentChunk) && 
		RoadNetwork->isIntersecting(pos) == false && 
		isInUndesireableTerrain(pos) == false 
		
		)
		return true;
	else
		return false;
}


void RoadManager::GenerateRoadChunk(Node* element, float plotGenerationChance, char dir, float length)
{
	sf::Vector2f pos;

	if(dir == 'R' || dir == 'L') 
		pos = sf::Vector2f(element->endPos.x + length, element->endPos.y );
	else
		pos = sf::Vector2f(element->endPos.x, element->endPos.y + length);


	if (checkRules(pos))
	{
		if (RoadNetwork->isOverlappingNode(pos) == false)
		{
			RoadNetwork->newLink(element, pos, dir);
		}
		else
		{
			RoadNetwork->connectNodes(RoadNetwork->getNode(pos), element);
		}
		//if (random(1, 100) <= plotGenerationChance)
		//{
		//	pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
		//	if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
		//	{
		//		plots.push_back(pos);
		//	}
		//	pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
		//	if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
		//	{
		//		plots.push_back(pos);
		//	}
		//}
	}
}

void RoadManager::applyRules(int iterations)
{
	float generationBias = 100;
	float plotGenerationChance = 10;
	int iterationNodes = 0;
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
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength);
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength);
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength);
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength);

						break;
					}
					case 'F':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength);

						break;
					}
					case 'R':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength);
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength);

						break;
					}
					case 'L':
					{
						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength);

						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength);

						if (random(1, 100) <= generationBias)
						GenerateRoadChunk(element, plotGenerationChance, 'F', roadLength);

						break;
					}
					case 'B':
					{

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'B', -roadLength);

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'R', roadLength);

						if (random(1, 100) <= generationBias)
							GenerateRoadChunk(element, plotGenerationChance, 'L', -roadLength);

						break;
					}



					default:
						break;
					}
				}
				else
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// STRAIGHT ROADS /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					if (*typeOfRoads == 2)
					{
						
					}
			}
			
				generationBias -= 1;
			element->rulesApplied = true;
		}
	
		RoadNetwork->CurrentNodes = RoadNetwork->IterationNodes;
		RoadNetwork->IterationNodes.clear();
		iterations--;
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
				sf::CircleShape circle(4);
				circle.setPosition(sf::Vector2f(element->endPos.x-circle.getRadius(), element->endPos.y - circle.getRadius()));
				circle.setFillColor(sf::Color::Black);
				refWindow->draw(circle);
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
							line->color = sf::Color(255, 255, 255, 10);
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

			/*for (auto element : plots)
			{

				sf::RectangleShape rectangle(sf::Vector2f(roadLength / 2, roadLength / 2));
				rectangle.setPosition(element);

				refWindow->draw(rectangle);
			}*/
		}
	}

	cout << numberOfRoads<<'\n';

	
}

void RoadManager::generatePlots(std::vector<Node*>AllNodes, Node* previousNode, Node* targetNode, int depth)
{
	for (auto element : AllNodes)
	{
		if (element->nodeLinks.size() > 0)
		{


			if (targetNode == NULL)
			{
				targetNode = element;	
			}


			if (targetNode != previousNode)
			{
				if (element == targetNode)
				{

				}
			}
			generatePlots(element->nodeLinks, element, targetNode, depth + 1);
		}
	}
	
	


}




