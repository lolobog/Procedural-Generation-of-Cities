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
                    std::cout << "Increased octave\n";
                      pNoise.nOctaveCount++;
                }

                if (pNoise.nOctaveCount == 9)
                    pNoise.nOctaveCount = 1;
            }


            if (event.type == sf::Event::Closed)
                window.close();
        }
        pNoise.PerlinNoise1D();

      
        window.clear(sf::Color::Black);

        pNoise.DrawPerlinNoise1D();
        gManager.DrawGrid();




        window.display();
    }

    return 0;


    return 0;
}
