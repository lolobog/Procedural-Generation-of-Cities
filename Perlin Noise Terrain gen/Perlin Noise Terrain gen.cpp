#include "PerlinNoise.h"


int main()
{
    srand(time(NULL));
    Singleton singleton;
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    GridManager gManager(&window);
    PerlinNoise pNoise(&gManager);
    PerlinNoise2D pn;
    int imgWidth = 1000;
    int imgHeight = 1000;
    sf::Image img;
    img.create(imgWidth, imgHeight);
    std::vector<int>colours;


    int kk = 0;
    int octaves = 8;
    for (int i = 0; i < imgWidth; i ++)
    {
        for (int j = 0; j < imgHeight; j ++)
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
          //  n = pn.noise(10 * x, 10 * y, 0.8);
           

            int bga;
            
                bga = floor(255 * n);
                colours.push_back(bga);

            sf::Color color;
            if (bga >= 450)
            {
                color.r = bga;
                color.g = bga;
                color.b = bga;
            }
            else
                if (bga >= 370)
                {
                    
                    color.g = 700- bga;
                   
                }
                else
                    if (bga >= 320)
                    {
                        color.r = bga +100;
                        color.g = bga +100;
                        color.b = bga;
                    }
                    else
                    {
                        
                        color.b = 255- bga;
                    }

          /*  color.r = bga;
            color.g = bga;
            color.b = bga;*/


            
            img.setPixel(i, j, color);
            kk++;

        }
        
    }
    sf::Texture noise;
    noise.loadFromImage(img);

   
   
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
