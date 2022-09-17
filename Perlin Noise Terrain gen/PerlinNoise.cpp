#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(GridManager* grid)
{
	pGrid = grid;
	outputSize = singleton.Instance()->ScreenWidth / grid->cellSize;
	for (int i = 0; i < outputSize; i++)
	{
		seed.push_back((float)rand() / (float)RAND_MAX);
	}
}

PerlinNoise::~PerlinNoise()
{
}

void PerlinNoise::PerlinNoise1D()
{
	if (noise.size() == outputSize)
		noise.clear();
	
		for (int i = 0; i < outputSize; i++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;
			for (int o = 0; o < nOctaveCount; o++)
			{
				int nPitch = outputSize >> o;
				int Sample1 = (i / nPitch) * nPitch;
				int Sample2 = (Sample1 + nPitch) % outputSize;

				float fBlend = (float)(i - Sample1) / (float)nPitch;
				float fSample = (1.0f - fBlend) * seed[Sample1] + fBlend * seed[Sample2];
				fNoise += fSample * fScale;
				fScaleAcc += fScale;
				fScale = fScale / 2.0f;
			}
			noise.push_back(fNoise / fScaleAcc);
		}
	

	
}

void PerlinNoise::DrawPerlinNoise1D()
{
	for (int i = 0; i < outputSize; i++)
	{
	
		for (int j = 0; j < ceil((noise[i] * outputSize )); j++)
		{
			pGrid->GridCells[outputSize/2-1-j][i].CellShape.setFillColor(sf::Color::Green);
		}
		
	}
}
