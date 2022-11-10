#include "RoadManager.h"

RoadManager::RoadManager(int chunkID,sf::RenderWindow* window)
{
	variables = { 'C' ,'F','R','L','B'};
	output = { 'C' };
	RoadNetwork->setRoot(findChunkCenter(chunkID),'C');
	currentChunk = chunkID;
	refWindow = window;
}

RoadManager::~RoadManager()
{
}

void RoadManager::applyRules(int iterations)
{
	

	while (iterations > 0)
	{
		

		for (auto element : RoadNetwork->AllNodes)
		{
			if (element->rulesApplied == false)
			{
				switch (element->nodeType)
				{
					
				case 'C':
				{
					if(isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y),currentChunk))//&&RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y))==false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x + roadLength, element->endPos.y), 'F');

					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');

					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk) )//&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x - roadLength, element->endPos.y), 'B');

					

					break;
				}
				case 'F':
				{
					if (isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y), currentChunk))//&&RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y))==false)
						RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x + roadLength, element->endPos.y), 'F');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					break;
				}
				case 'R':
				{
					if (isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.y + roadLength, element->endPos.y), 'F');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					break;
				}
				case 'L':
				{
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.y - roadLength, element->endPos.y), 'B');

					break;
				}
				case 'B':
				{
					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk))//&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
						RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x - roadLength, element->endPos.y), 'B');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk) )//&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk) )//&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					break;
				}



				default:
					break;
				}
			}
			element->rulesApplied = true;
		}
		
		iterations--;
	}
}

void RoadManager::drawRoads()
{
	for (auto element : RoadNetwork->AllNodes)
	{
		if (element->parent != NULL)
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


