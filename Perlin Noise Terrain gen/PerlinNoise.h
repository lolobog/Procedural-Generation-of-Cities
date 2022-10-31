#pragma once
#include "Grid.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

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


	void PerlinNoise1D();
	void DrawPerlinNoise1D();


};

//https://github.com/sol-prog/Perlin_Noise - sol-prog Paul Silisteanu
class PerlinNoise2D
{
	std::vector<int> p;
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise2D();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise2D(unsigned int seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);
	

	
};