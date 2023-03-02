#include "RoadManager.h"

RoadManager::RoadManager(int chunkID,sf::RenderWindow* window,int roadsType, PerlinNoise2D* pn)
{
	perlinRef = pn;
	variables = { 'C' ,'F','R','L','B'};
	output = { 'C' };
	RoadNetwork->setRoot(findChunkCenter(chunkID),'C');
	currentChunk = chunkID;
	refWindow = window;
	rType = roadsType;
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





void RoadManager::applyRules(int iterations)
{
	float generationBias = 100;
	

	while (iterations > 0)
	{
		
		
		for (int i = 0; i < RoadNetwork->CurrentNodes.size(); i++)
		{
			Node* element = RoadNetwork->CurrentNodes[i];

			int positionsProcessed = 0;
			 
				if (element->rulesApplied == false)
				{
					switch (element->nodeType)
					{

					case 'C':
					{
						sf::Vector2f pos;
						pos = sf::Vector2f(element->endPos.x , element->endPos.y + roadLength);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
							RoadNetwork->newLink(element, pos, 'F');

						pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
							RoadNetwork->newLink(element, pos, 'B');
				
						pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y );
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
							RoadNetwork->newLink(element, pos, 'R');
				
						pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y );
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
							RoadNetwork->newLink(element, pos, 'L');
			
					

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());

						break;
					}
					case 'F':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false&&isInUndesireableTerrain(pos)==false)
								RoadNetwork->newLink(element, pos, 'F');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'L');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'R');
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());

						break;
					}
					case 'R':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'F');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'R');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'B');
						}


						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						break;
					}
					case 'L':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'B');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'L');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'F');
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						break;
					}
					case 'B':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'B');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'R');
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
								RoadNetwork->newLink(element, pos, 'L');
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						break;
					}



					default:
						break;
					}
				}
			
				generationBias -= 1;
			element->rulesApplied = true;
		}
	
		iterations--;
	}
}

void RoadManager::drawRoads()
{
	if (this != NULL)
	{
		for (auto element : RoadNetwork->AllNodes)
		{
			if (element->parent != nullptr)
			{

				sf::Vertex line[] =
				{
					sf::Vertex(element->parent->endPos),
					sf::Vertex(element->endPos)
				};
				refWindow->draw(line, 2, sf::Lines);
			}
		}
	}
}




