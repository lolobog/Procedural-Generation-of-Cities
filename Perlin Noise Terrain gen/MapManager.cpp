#include "MapManager.h"
#include <iostream>

MapManager::MapManager(PerlinNoise2D* pn)
{
    perlinRef = pn;
}

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

std::vector<double> MapManager::calcChunkAvHeight( std::vector<double>noiseLevels)
{
	std::vector<double>chunkAvHeight;
	for (int x = 0; x < imgWidth; x += chunkWidth)
	{
		for (int y = 0; y < imgHeight; y += chunkHeight)
		{
			int k = 0;
			double value = 0;
			for (int i = x; i < x + chunkWidth; i++)
			{
				for (int j = y; j < y + chunkHeight; j++)
				{
					value += noiseLevels[imgWidth * j + i];
					k++;
				}
			}
			chunkAvHeight.push_back(value / k);
		}

	}
	return chunkAvHeight;

}
int MapManager::getBestChunkID(std::vector<double>chunkAvHeight)
{
    int bestValueID = 0;
    double bestValue = DBL_MAX;

    for (int chunkID = 0; chunkID < chunkAvHeight.size(); chunkID++)
    {
        int scalingFactor = 1;
        if (chunkAvHeight[chunkID] < 80)
            scalingFactor = 100;
        if (chunkAvHeight[chunkID] >= 80 && chunkAvHeight[chunkID] <= 85)
            scalingFactor = 5;
        if (chunkAvHeight[chunkID] >= 85 && chunkAvHeight[chunkID] <= 110)
        {
            scalingFactor = 2;
        }
        if (chunkAvHeight[chunkID] > 150)
            scalingFactor = 10;



        if (bestValue > chunkAvHeight[chunkID] * scalingFactor)
        {
            bestValue = chunkAvHeight[chunkID] * scalingFactor;
            bestValueID = chunkID;
        }
    }
    return bestValueID;
}
void MapManager::colorImg(sf::Image& img, std::vector<double>mappedValues)
{
    for (int i = 0; i < mappedValues.size(); ++i)
    {

        sf::Color color;

        if (mappedValues[i] >= 150)
        {
            color.r = mappedValues[i];
            color.g = mappedValues[i];
            color.b = mappedValues[i];
        }
        else
            if (mappedValues[i] >= 85)
            {

                color.g = mappedValues[i];

            }
            else
                if (mappedValues[i] >= 80)
                {
                    color.r = mappedValues[i] + 120;
                    color.g = mappedValues[i] + 120;
                    color.b = mappedValues[i];
                }
                else
                {

                    color.b = mappedValues[i] + 155;
                }




        int y = i % imgWidth;
        int x = i / imgHeight;

        img.setPixel(x, y, color);
    }
}

int MapManager::findChunkX(int bestValueID)
{
    
    return (bestValueID % (imgWidth / chunkWidth)) * chunkWidth;
}

int MapManager::findChunkY(int bestValueID)
{
    return (bestValueID / (imgHeight / chunkHeight)) * chunkHeight;
}

sf::Vector2f MapManager::findChunkStartP(int bestValueID)
{
    return sf::Vector2f((bestValueID % (imgWidth / chunkWidth)) * chunkWidth, (bestValueID / (imgHeight / chunkHeight)) * chunkHeight);
}

sf::Vector2f MapManager::findChunkCenter(int bestValueID)
{
    
    return sf::Vector2f(findChunkX(bestValueID)+chunkWidth/2, findChunkY(bestValueID)+chunkHeight/2);
}



bool MapManager::isInChunkBounds(sf::Vector2f point,int chunkID)
{
   

    if (point.x > findChunkX(chunkID) && point.y > findChunkY(chunkID) && point.x < findChunkX(chunkID) + chunkWidth && point.y < findChunkY(chunkID) + chunkHeight)
    {
        
        return true;
   
    }
    else
    {
         
       return false;
    }
}

bool MapManager::isInUndesireableTerrain(sf::Vector2f point)
{
    if (perlinRef->noiseValues[point.x * imgWidth+point.y] > 80 && perlinRef->noiseValues[point.x * imgWidth + point.y] < 150)
    {
       
        return false;
    }


    return true;

    
}




