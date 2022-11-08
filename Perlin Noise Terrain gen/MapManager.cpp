#include "MapManager.h"

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

std::vector<double> MapManager::calcChunkAvHeight(int chunkWidth, int chunkHeight, std::vector<double>noiseLevels, int imgWidth, int imgHeight)
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
void MapManager::colorImg(sf::Image& img, int imgWidth, int imgHeight, std::vector<double>mappedValues)
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