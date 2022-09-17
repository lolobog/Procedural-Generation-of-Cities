#pragma once
#include "Grid.h"

class PerlinNoise
{
private:
	GridManager* pGrid;


public:
	PerlinNoise(GridManager* grid);
	~PerlinNoise();
	Singleton singleton;
	
	int outputSize ;
	std::vector <float> seed;
	std::vector<float>noise;

	int nOctaveCount = 1;


	void PerlinNoise1D();

	void DrawPerlinNoise1D();

	
	


};