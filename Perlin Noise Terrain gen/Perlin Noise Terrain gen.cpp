#include "PerlinNoise.h"


int main()
{
    srand(time(NULL));
    Singleton singleton;
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    GridManager gManager(&window);
    PerlinNoise pNoise(&gManager);
    PerlinNoise2D pn(195);
    int imgWidth = 1000;
    int imgHeight = 1000;
    sf::Image img;
    img.create(imgWidth, imgHeight);



    int kk = 0;
    int octaves = 2;
    for (int i = 0; i < imgWidth; i ++)
    {
        for (int j = 0; j < imgHeight; j ++)
        {
            
            double x = (double)j / ((double)imgWidth);
            double y = (double)i / ((double)imgHeight);

            // Typical Perlin noise
            double n = 0;
            double a = 1.0f;
            double f = 0.005f;

            for (int o = 0; o < octaves; o++)
            {
                double v = a * pn.noise(x * f, y * f, 0.8);
                n += v;
                a *= 0.5;
                f *= 2.0;
            }
            n = pn.noise(10 * x, 10 * y, 0.8);

            int bga = floor(255 * n);
            sf::Color color;
            if (bga >= 140)
            {
                color.r = 255-floor(255 * n);
                color.g = 255 - floor(255 * n) ;
                color.b = 255 - floor(255 * n) ;
            }
            else
                if (bga >= 70)
                {
                    
                    color.g = 300-floor(255 * n);
                   
                }
                else
                    if (bga >= 50)
                    {
                        color.r = floor(255 * n)+100;
                        color.g = floor(255 * n)+100;
                        color.b = floor(255 * n);
                    }
                    else
                    {
                        
                        color.b = 255-floor(255 * n);
                    }




            
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
