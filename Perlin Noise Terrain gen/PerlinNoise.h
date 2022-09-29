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
	int nOctaveCount = 1;
	float bias = 2;

	// For 1D generation
	int outputSize;
	std::vector <float> seed;
	std::vector<float>noise;

	//For 2D generation
	int outputWitdth = singleton.Instance()->ScreenWidth;
	int outputHeight = singleton.Instance()->ScreenHeight;
	std::vector <float> seed2D;
	float* noise2D = new float[outputHeight * outputWitdth];
	float* noise2D2 = new float[.\][outputWitdth];
	
	

	void PerlinNoise1D();

	void PerlinNoise2D();

	void DrawPerlinNoise1D();

	void DrawPerlinNoise2D();

	
	


};