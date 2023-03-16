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





void RoadManager::GenerateRoadChunk(Node* element, float plotGenerationChance, char dir, float length)
{
	sf::Vector2f pos;

	if(dir == 'F' || dir == 'B') 
		pos = sf::Vector2f(element->endPos.x + length, element->endPos.y );
	else
		pos = sf::Vector2f(element->endPos.x, element->endPos.y + length);

	if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
	{
		RoadNetwork->newLink(element, pos, dir);
		if (random(1, 100) <= plotGenerationChance)
		{
			pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
			if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
			{
				plots.push_back(pos);
			}
			pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
			if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
			{
				plots.push_back(pos);
			}
		}
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
	
		if (iterationNodes == 0)
		{
			iterationNodes = 4;
		}
		else
		{
			iterationNodes = iterationNodes * 3;
		}
		
		for (i ; i < RoadNetwork->CurrentNodes.size(); i++)
		{
			std::cout << iterationNodes+'\n';
			if (i > iterationNodes)
			{
				std::cout << "Reached iteration limit \n";
				break;
			}
			Node* element = RoadNetwork->CurrentNodes[i];

			 
			if (element->rulesApplied == false)
			{
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

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;

						break;
					}
					case 'F':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;

						break;
					}
					case 'R':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}


						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}
					case 'L':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}
					case 'B':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}



					default:
						break;
					}
				}
				else
					if (*typeOfRoads == 2)
					{
						switch (element->nodeType)
					{

					case 'C':
					{
						sf::Vector2f pos;
						pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
						{
							RoadNetwork->newLink(element, pos, 'F');
							if (random(1, 100) <= plotGenerationChance)
							{
								pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
								pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
							}
						}

						pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
						{
							RoadNetwork->newLink(element, pos, 'B');

							if (random(1, 100) <= plotGenerationChance)
							{
								pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
								pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
							}
						}

						pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
						{
							RoadNetwork->newLink(element, pos, 'R');
							if (random(1, 100) <= plotGenerationChance)
							{
								pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
								pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
							}
						}

						pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
						if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false)
						{
							RoadNetwork->newLink(element, pos, 'L');
							if (random(1, 100) <= plotGenerationChance)
							{
								pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
								pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
								if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
								{
									plots.push_back(pos);
								}
							}
						}



						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;

						break;
					}
					case 'F':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;

						break;
					}
					case 'R':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y + roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}


						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}
					case 'L':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'F');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}
					case 'B':
					{
						sf::Vector2f pos;

						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x, element->endPos.y - roadLength);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'B');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x + roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'R');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x + roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}
						if (random(1, 100) <= generationBias)
						{
							pos = sf::Vector2f(element->endPos.x - roadLength, element->endPos.y);
							if (isInChunkBounds(pos, currentChunk) && RoadNetwork->isIntersecting(pos) == false && isInUndesireableTerrain(pos) == false)
							{
								RoadNetwork->newLink(element, pos, 'L');
								if (random(1, 100) <= plotGenerationChance)
								{
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y + roadLength / 2);
									if (isPositionPlotted(pos) == false && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
									pos = sf::Vector2f(element->endPos.x - roadLength / 2, element->endPos.y - roadLength / 2);
									if (isPositionPlotted(pos) && isInUndesireableTerrain(pos) == false)
									{
										plots.push_back(pos);
									}
								}
							}
						}

						RoadNetwork->CurrentNodes.erase(RoadNetwork->CurrentNodes.begin());
						i--;
						break;
					}



					default:
						break;
					}
					}
			}
			
				generationBias -= 0.5;
			element->rulesApplied = true;
		}
	
		iterations--;
	}
}

void RoadManager::drawRoads()
{
	if (this != nullptr)
	{
		if (perlinRef != nullptr)
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

			for (auto element : plots)
			{

				sf::RectangleShape rectangle(sf::Vector2f(roadLength / 2, roadLength / 2));
				rectangle.setPosition(element);

				refWindow->draw(rectangle);
			}
		}
	}
}




