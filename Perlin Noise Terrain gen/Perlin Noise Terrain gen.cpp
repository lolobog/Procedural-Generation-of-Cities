#include "PerlinNoise.h"

#include <iostream>

int map(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

 std::vector<double> generateNoise(PerlinNoise2D pn,int octaves,std::vector<double>noiseLevels,int imgWidth,int imgHeight)
{
    for (int i = 0; i < imgWidth; i++)
    {
        for (int j = 0; j < imgHeight; j++)
        {

            double x = (double)j / ((double)imgWidth);
            double y = (double)i / ((double)imgHeight);

            // Typical Perlin noise
            double n = 0;
            double a = 1.0f;
            double f = 0.9f;

            for (int o = 0; o < octaves; o++)
            {
                double v = a * pn.noise(x * f, y * f, 0.8);
                n += v;
                a *= 0.7;
                f *= 2;
            }
            noiseLevels.push_back(n);
        }
    }
    return noiseLevels;
}

 std::vector<double> blendNoise(std::vector<double>noiseCopy,int blendLvl,int imgHeight,int imgWidth)
 {
     for (int i = 0; i < imgHeight; ++i)
     {
         for (int j = 0; j < imgWidth; ++j)
         {
             // width * row + col
             double value = 0;
             int k = 0;
             if (i >= blendLvl && j >= blendLvl && i < imgHeight - blendLvl && j < imgWidth - blendLvl)
             {

                 for (int p = i - blendLvl; p < i + blendLvl; p++)
                 {
                     for (int q = j - blendLvl; q < j + blendLvl; q++)
                     {
                         if (p != q)
                         {
                             value += noiseCopy[1000 * q + p];
                             k++;
                         }
                     }
                 }




                 noiseCopy[1000 * j + i] = value / k;
             }


             if (i <= blendLvl)
             {
                 for (int p = blendLvl - i; p < imgWidth && p <= blendLvl + i; p++)
                 {
                     if (j <= blendLvl)
                     {
                         for (int q = blendLvl - j; q < imgHeight && q <= blendLvl + j; q++)
                         {
                             if (p != i && q != j)
                             {
                                 value += noiseCopy[1000 * q + p];
                                 k++;
                             }
                         }
                     }

                     if (j > blendLvl)
                     {
                         for (int q = j - blendLvl; q < imgHeight && q <= blendLvl + j; q++)
                         {
                             if (p != i && q != j)
                             {
                                 value += noiseCopy[1000 * q + p];
                                 k++;
                             }
                         }
                     }

                     if (j >= imgWidth - blendLvl)
                     {
                         for (int q = j - blendLvl; q < imgHeight && q <= blendLvl + j; q++)
                         {
                             if (p != i && q != j)
                             {
                                 value += noiseCopy[1000 * q + p];
                                 k++;
                             }
                         }
                     }
                 }
                 noiseCopy[1000 * j + i] = value / k;
             }
             else
                 if (i >= imgHeight - blendLvl || (i > blendLvl && j <= blendLvl) || (j > blendLvl && i <= imgWidth - blendLvl))
                 {
                     for (int p = i - blendLvl; p < imgWidth && p <= blendLvl + i; p++)
                     {
                         if (j <= blendLvl)
                         {
                             for (int q = blendLvl - j; q < imgHeight && q <= blendLvl + j; q++)
                             {
                                 if (p != i && q != j)
                                 {
                                     value += noiseCopy[1000 * q + p];
                                     k++;
                                 }
                             }
                         }

                         if (j > blendLvl)
                         {
                             for (int q = j - blendLvl; q < imgHeight && q <= blendLvl + j; q++)
                             {
                                 if (p != i && q != j)
                                 {
                                     value += noiseCopy[1000 * q + p];
                                     k++;
                                 }
                             }
                         }

                         if (j >= imgWidth - blendLvl)
                         {
                             for (int q = j - blendLvl; q < imgHeight && q <= blendLvl + j; q++)
                             {
                                 if (p != i && q != j)
                                 {
                                     value += noiseCopy[1000 * q + p];
                                     k++;
                                 }
                             }
                         }

                     }
                     noiseCopy[1000 * j + i] = value / k;
                 }


         }
     }
     return noiseCopy;
 }

 std::vector<double> calcChunkAvHeight(int chunkWidth,int chunkHeight,std::vector<double>noiseLevels,int imgWidth, int imgHeight)
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
                     value += noiseLevels[1000 * j + i];
                     k++;
                 }
             }
             chunkAvHeight.push_back(value / k);
         }
         
     }
     return chunkAvHeight;
     
 }
 void colorImg(sf::Image &img,int imgWidth,int imgHeight,std::vector<double>mappedValues)
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

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    PerlinNoise2D pn(189);
    int imgWidth = 1000;
    int imgHeight = 1000;
    sf::Image img;
    img.create(imgWidth, imgHeight);

    int octaves = 8;
    int blendLvl = 8;

    float low = FLT_MAX;
    float high = 0.0f;

    std::vector<double> noiseLevels= generateNoise(pn, octaves, noiseLevels, imgWidth, imgHeight); //Generate the perlin noise
    noiseLevels = blendNoise(noiseLevels,blendLvl,imgHeight,imgWidth); //Blend the perlin noise to look more uniform

    std::vector<double> mappedValues;
    for (int i = 0; i < noiseLevels.size(); ++i)
    {
        mappedValues.push_back(map(noiseLevels[i], 1, 2, 0, 255));
    }
    colorImg(img, imgWidth, imgHeight, mappedValues);

    sf::Texture noise;
    noise.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(noise);

    

    int chunkWidth = 200, chunkHeight=200;
    std::vector<double>chunkAvHeight=calcChunkAvHeight(chunkWidth,chunkHeight,noiseLevels,imgWidth,imgHeight);//Calculate the average height of every chunk

 
    
    int bestValueID;
    double bestValue = DBL_MAX;
    for (int chunkID = 0;chunkID<chunkAvHeight.size();chunkID++)
    {
        if (bestValue > chunkAvHeight[chunkID])
        {
            bestValue = chunkAvHeight[chunkID];
            bestValueID = chunkID;
        }
    }

    std::cout <<"Size of the array of chunks: "<< chunkAvHeight.size() << '\n';
    std::cout << "Best chunk for city building: " << bestValue << '\n';
    std::cout << "ID of the best chunks: " << bestValueID << '\n';
    std::cout << "Chuck start point X: " << (bestValueID % (imgWidth/chunkWidth)) * chunkWidth << " Y: " << (bestValueID /(imgHeight/chunkHeight)) * chunkHeight << '\n';
    
    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth,chunkHeight));
    chunkOutline.setOutlineColor(sf::Color::Black);
    chunkOutline.setOutlineThickness(10);
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setPosition((bestValueID % (imgWidth / chunkWidth)) * chunkWidth, (bestValueID / (imgHeight / chunkHeight)) * chunkHeight);

  

   
    while (window.isOpen())
    {
    
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                   
                    if (octaves == 9)
                    {
                        octaves = 1;
                    }
                    else
                        octaves++;

                      std::cout << "Increased octave\n";
                }

                

                if (event.key.code == sf::Keyboard::Z)
                {
                   
                 
                    std::cout << "Reset seed\n";
                }

                if (event.key.code == sf::Keyboard::W)
                {
                   
                    
                }
                if (event.key.code == sf::Keyboard::S)
                {
                  
                }

            }


            if (event.type == sf::Event::Closed)
                window.close();
        }
      
      
      
        window.clear(sf::Color::Black);

        window.draw(sprite);
        window.draw(chunkOutline);



        window.display();
    }

    return 0;

}
