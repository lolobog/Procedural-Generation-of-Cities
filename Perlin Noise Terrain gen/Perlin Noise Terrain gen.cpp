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
    int blendLvl = 5;

    for (int i = 0; i < imgHeight; ++i)
    {
        for (int j = 0; j < imgWidth; ++j)
        {
            // width * row + col
            
            if (i >= blendLvl && j >= blendLvl && i < imgHeight- blendLvl && j < imgWidth- blendLvl)
            {
                double value=0;
                int k = 0;
                for (int p = i - blendLvl; p < i + blendLvl; p++)
                {
                    for (int q = j - blendLvl; q < j + blendLvl; q++)
                    {
                        if (p!= q)
                        {
                            value += noiseCopy[1000 * q + p];
                            k++;
                        }
                    }
                }


              //   std::cout << "Averaged value: " << value << "\n";
                noiseCopy[1000 * j + i]=value/k;
            }

        }
    }
    noiseLevels = noiseCopy;

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
                    if (bga >= 75)
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




        window.display();
    }

    return 0;


    return 0;
}
