#include "RoadManager.h"


int mapVal(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}
std::vector<double> mapVector(std::vector<double> noiseLevels,float start1, float stop1, float start2, float stop2)
{
    std::vector<double> mappedValues;
    for (int i = 0; i < noiseLevels.size(); ++i)
    {
        mappedValues.push_back(mapVal(noiseLevels[i], start1, stop1, start2, stop2));
    }
    return mappedValues;
}

int main()
{
    
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    PerlinNoise2D pn(86);
    MapManager mapM;
   
    sf::Image img;
    img.create(imgWidth, imgHeight);

    int octaves = 8;
    int blendLvl = 8;

    std::vector<double> noiseLevels= pn.generateNoise(pn, octaves, noiseLevels); //Generate the perlin noise
    noiseLevels = pn.blendNoise(noiseLevels,blendLvl); //Blend the perlin noise to look more uniform

    noiseLevels = mapVector(noiseLevels,1,2,0,255);
    mapM.colorImg(img, noiseLevels);

    sf::Texture noise;
    noise.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(noise);

    

    
    std::vector<double>chunkAvHeight= mapM.calcChunkAvHeight( noiseLevels);//Calculate the average height of every chunk
 
    
    int bestValueID=mapM.getBestChunkID(chunkAvHeight);
  

    std::cout <<"Size of the array of chunks: "<< chunkAvHeight.size() << '\n';
    std::cout << "ID of the best chunks: " << bestValueID << '\n';
    std::cout << "Chuck start point X: " << mapM.findChunkX(bestValueID) << " Y: " << mapM.findChunkY(bestValueID) << '\n';
    
    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth, chunkHeight));
    chunkOutline.setOutlineColor(sf::Color::Black);
    chunkOutline.setOutlineThickness(10);
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setPosition(mapM.findChunkStartP(bestValueID));
   
    RoadManager roads(bestValueID,&window);
    roads.applyRules(20);
  

    
   
  

   
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

        roads.drawRoads();
       

        window.display();
    }

    return 0;

}
