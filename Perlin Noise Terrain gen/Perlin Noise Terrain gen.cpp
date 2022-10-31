#include "PerlinNoise.h"

#include <iostream>

int map(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

int main()
{
    srand(time(NULL));
    Singleton singleton;
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    GridManager gManager(&window);
    PerlinNoise pNoise(&gManager);
    PerlinNoise2D pn(189);
    int imgWidth = 1000;
    int imgHeight = 1000;
    sf::Image img;
    img.create(imgWidth, imgHeight);
    std::vector<int>colours;


    int kk = 0;
    int octaves = 8;

    float low = FLT_MAX;
    float high = 0.0f;

    std::vector<double> noiseLevels;

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

    std::vector<double> noiseCopy = noiseLevels;
    int blendLvl = 8;

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
                    if (i >= imgHeight - blendLvl || (i>blendLvl&&j<=blendLvl)|| (j > blendLvl && i <= imgWidth - blendLvl))
                    {
                        for (int p = i-blendLvl; p < imgWidth && p <= blendLvl + i; p++)
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


                
            
              //   std::cout << "Averaged value: " << value << "\n";
              //  noiseCopy[1000 * j + i]=value/k;
            

        }
    }
    noiseLevels = noiseCopy;

    int chunkWidth = 100, chunkHeight=100;
    std::vector<double>chunkAvHeight;

    for (int x = 0; x < imgWidth; x += chunkWidth)
    {
        for (int y = 0; y < imgHeight; y += chunkHeight)
        {
            int k = 0;
            double value=0;
            for (int i = x; i < x+chunkWidth; i++)
            {
                for (int j = y; j < y+chunkHeight; j++)
                {
                    value+=noiseLevels[1000 * j + i];
                    k++;
                }
            }
            chunkAvHeight.push_back(value / k);
        }
    }
    
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

    for(int i = 0; i < noiseLevels.size(); ++i)
    {
        double n = noiseLevels[i];

            int index = map(n, 1, 2, 0, 255);

            if (n < low)
                low = n;

            if (n > high)
                high = n;

            int bga;
            
            bga = index;// floor(255 * n);
                colours.push_back(bga);

            sf::Color color;
            if (bga >= 150)
            {
                color.r = bga;
                color.g = bga;
                color.b = bga;
            }
            else
                if (bga >= 85)
                {
                    
                    color.g = bga;
                   
                }
                else
                    if (bga >= 80)
                    {
                        color.r = bga + 120;
                        color.g = bga + 120;
                        color.b = bga;
                    }
                    else
                    {
                        
                        color.b = bga + 155;
                    }

       


            int y = i % imgWidth;
            int x = i / imgHeight;

            img.setPixel(x, y, color);
            kk++;        
    }
    sf::Texture noise;
    noise.loadFromImage(img);

    std::cout << "lowest noise: " << low << "\nhighest noise: " << high << '\n';
   
    sf::Sprite sprite;
    sprite.setTexture(noise);

   
    while (window.isOpen())
    {
    
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                   
                      pNoise.nOctaveCount++;
                      std::cout << "Increased octave\n";
                }

                if (pNoise.nOctaveCount == 9)
                    pNoise.nOctaveCount = 1;

                if (event.key.code == sf::Keyboard::Z)
                {
                    pNoise.seed.clear();
                    gManager.ClearGridColor();
                    for (int i = 0; i < pNoise.outputSize; i++)
                    {
                        pNoise.seed.push_back((float)rand() / (float)RAND_MAX);
                        
                    }
                 
                    std::cout << "Reset seed\n";
                }

                if (event.key.code == sf::Keyboard::W)
                {
                    pNoise.bias += 0.2;
                    gManager.ClearGridColor();
                    
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    pNoise.bias += -0.2;
                    gManager.ClearGridColor();
                }

            }


            if (event.type == sf::Event::Closed)
                window.close();
        }
      
      //  pNoise.PerlinNoise1D();
      
        window.clear(sf::Color::Black);
        window.draw(sprite);
     
      //  pNoise.DrawPerlinNoise1D();
      //  gManager.DrawGrid();
        window.draw(chunkOutline);



        window.display();
    }

    return 0;


    return 0;
}
