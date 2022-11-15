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


int RoadManager::random(int low, int high)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);

}



void RoadManager::applyRules(int iterations)
{
	

	

	while (iterations > 0)
	{
		

		for (auto element : RoadNetwork->CurrentNodes)
		{
			if (element->rulesApplied == false)
			{
				switch (element->nodeType)
				{
					
				case 'C':
				{

					//if(random(1,2)==2)
					if(isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y),currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y))==false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x + roadLength, element->endPos.y), 'F');
					//if (random(1, 2) == 2)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					//if (random(1, 2) == 2)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');
					//if (random(1, 2) == 2)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x - roadLength, element->endPos.y), 'B');

					RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());

					break;
				}
				case 'F':
				{
					if (random(1, 3) == 2|| random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y), currentChunk) &&RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y))==false)
						RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x + roadLength, element->endPos.y), 'F');
					if (random(1, 3) == 2 || random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (random(1, 3) == 2 || random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());

					break;
				}
				case 'R':
				{
					if (random(1, 3) == 3|| random(1, 3) == 2)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x + 10, element->endPos.y), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x + roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.y + roadLength, element->endPos.y), 'F');
					if (random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');
					if (random(1, 3) == 3 || random(1, 3) == 2)
						if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
							RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');

					RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
					break;
				}
				case 'L':
				{
					if (random(1, 3) == 3 || random(1, 3) == 2)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk)&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (random(1,3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.y - roadLength, element->endPos.y), 'B');
					if (random(1, 3) == 3 || random(1, 3) == 2)
						if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk) && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
							RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
					break;
				}
				case 'B':
				{
					if (random(1, 2) == 2 || random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x - 10, element->endPos.y), currentChunk))//&& RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x - roadLength, element->endPos.y)) == false)
						RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x - roadLength, element->endPos.y), 'B');
					if (random(1, 2) == 2 || random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y + 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y + roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y + roadLength), 'R');
					if (random(1, 2) == 2 || random(1, 3) == 3)
					if (isInChunkBounds(sf::Vector2f(element->endPos.x, element->endPos.y - 10), currentChunk))// && RoadNetwork->isIntersecting(sf::Vector2f(element->endPos.x, element->endPos.y - roadLength)) == false)
					RoadNetwork->newLink(element, sf::Vector2f(element->endPos.x, element->endPos.y - roadLength), 'L');

					RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
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




