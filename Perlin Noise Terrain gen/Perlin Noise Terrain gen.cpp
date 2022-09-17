#include "Grid.h"


int main()
{
    Singleton singleton;

   

  
    sf::RenderWindow window(sf::VideoMode(singleton.Instance()->ScreenWidth, singleton.Instance()->ScreenHeight), "Perlin Noise Terrain");
    GridManager gManager(&window);
  
    while (window.isOpen())
    {
    
        sf::Event event;
        while (window.pollEvent(event))
        {
        
            if (event.type == sf::Event::Closed)
                window.close();
        }

      
        window.clear(sf::Color::Black);


        gManager.DrawGrid();




        window.display();
    }

    return 0;


    return 0;
}
