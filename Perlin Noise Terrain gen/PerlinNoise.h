#pragma once
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <SFML/Graphics.hpp>
#include "Variables.h"

//https://github.com/sol-prog/Perlin_Noise - sol-prog Paul Silisteanu
class PerlinNoise2D
{
	std::vector<int> p;
	std::vector<int> initialPermutationV;
private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise2D();
	// Generate a new permutation vector based on the value of seed
	PerlinNoise2D(int* seed);
	// Get a noise value, for 2D images z can have any value
	double noise(double x, double y, double z);
	std::vector<double> blendNoise(std::vector<double>noiseCopy, int blendLvl);
	std::vector<double> generateNoise(PerlinNoise2D pn, int octaves, std::vector<double>noiseLevels);
	void initializeRandomEngine(int* seed);
	void generateNoiseWithSeed(int* seed);

	std::vector<double> noiseValues;
	int* givenSeed;
	

	
};