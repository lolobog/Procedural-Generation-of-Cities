#pragma once
#include "PerlinNoise.h"


class MapManager
{
public:
	
	MapManager();
	~MapManager();
	
	std::vector<double> calcChunkAvHeight( std::vector<double>noiseLevels);
	int getBestChunkID(std::vector<double>chunkAvHeight);
	void colorImg(sf::Image& img, std::vector<double>mappedValues);
	int findChunkX(int bestValueID);
	int findChunkY(int bestValueID);
	sf::Vector2f findChunkCenter(int bestValueID);
	sf::Vector2f findChunkStartP(int bestValueID);
	bool isInChunkBounds(sf::Vector2f point,int chunkID);
	


private:
};


