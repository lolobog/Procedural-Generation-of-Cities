#include "PerlinNoise.h"



int main()
{
    Singleton singleton;
    sf::RenderWindow window(sf::VideoMode(singleton.Instance()->ScreenWidth, singleton.Instance()->ScreenHeight), "Perlin Noise Terrain");
    GridManager gManager(&window);
    PerlinNoise pNoise(&gManager);
  
   
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
       // pNoise.PerlinNoise1D();
        pNoise.PerlinNoise2D();
      
        window.clear(sf::Color::Black);

      //  pNoise.DrawPerlinNoise1D();
        pNoise.DrawPerlinNoise2D();
        gManager.DrawGrid();




        window.display();
    }

    return 0;


    return 0;
}
