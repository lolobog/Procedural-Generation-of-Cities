#pragma once
#include "PerlinNoise.h"
class MapManager
{
public:
	MapManager();
	~MapManager();

	std::vector<double> calcChunkAvHeight(int chunkWidth, int chunkHeight, std::vector<double>noiseLevels, int imgWidth, int imgHeight);
	void colorImg(sf::Image& img, int imgWidth, int imgHeight, std::vector<double>mappedValues);

private:
};


