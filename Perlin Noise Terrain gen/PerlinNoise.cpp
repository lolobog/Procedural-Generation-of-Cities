#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(GridManager* grid)
{
	pGrid = grid;
	outputSize = ScreenWidth / grid->cellSize;
	for (int i = 0; i < outputSize; i++)
	{
		seed.push_back((float)rand() / (float)RAND_MAX);
	}

	for (int j = 0; j < outputHeight * outputWitdth; j++)
	{
		seed2D.push_back((float)rand() / (float)RAND_MAX);
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
				fScale = fScale / bias;
			}
			noise.push_back(fNoise / fScaleAcc);
		}
	

	
}


void PerlinNoise::PerlinNoise2D()
{
	/*if (noise2D.size() == outputHeight * outputWitdth)
		noise2D.clear();*/

	for (int x = 0; x < outputWitdth; x++)
	{
		for (int y = 0; y < outputHeight; y++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;
			for (int o = 0; o < nOctaveCount; o++)
			{
				int nPitch = outputWitdth >> o;

				int Sample1X = (x / nPitch) * nPitch;
				int Sample1Y = (y / nPitch) * nPitch;

				int Sample2X = (Sample1X + nPitch)% outputWitdth;
				int Sample2Y = (Sample1Y + nPitch)% outputHeight;

				float fBlendX = (float)(x - Sample1X) / (float)nPitch;
				float fBlendY = (float)(y - Sample1Y) / (float)nPitch;

				float fSample1 = (1.0f - fBlendX) * seed2D[Sample1Y* outputWitdth +Sample1X] + fBlendX * seed2D[Sample1Y * outputWitdth + Sample2X];
				float fSample2 = (1.0f - fBlendX)* seed2D[Sample2Y * outputWitdth + Sample2X] + fBlendX * seed2D[Sample2Y * outputWitdth + Sample2X];

				fNoise += (fBlendY * (fSample2 - fSample1) + fSample1) * fScale;
				fScaleAcc += fScale;
				fScale = fScale / bias;
			}
			noise2D[y*outputWitdth+x]=(fNoise / fScaleAcc);
		}
	}



}

void PerlinNoise::DrawPerlinNoise1D()
{
	for (int i = 0; i < outputSize; i++)
	{
	
		for (int j = 0; (j < ceil(noise[i] * outputSize )); j++)
		{
			pGrid->GridCells[outputSize-j-1][i].CellShape.setFillColor(sf::Color::Green);
		}
		
	}
}

void PerlinNoise::DrawPerlinNoise2D()
{
	

	for (int x = 0; x < outputSize; x++)
	{
		
		for (int y = 0;y < outputSize; y++)
		{
			uint8_t col = noise2D[y * outputWitdth + x] * 255;
			sf::Color cellColor(col, col, col, 255);
			pGrid->GridCells[x][y].CellShape.setFillColor(cellColor);
			
		}
		std::cout << "Line drawn\n";
	}

	//for (auto line : pGrid->GridCells)
	//{
	//	for (auto cell : line)
	//	{
	//		sf::Color cellColor(noise2D[x] * 100, noise2D[x] * 100, noise2D[x] * 100, 255);
	//		cell.CellShape.setFillColor(cellColor);
	//		x++;
	//	}
	//}
}
