#include "MapManager.h"
#include "LSystem.h"
#include <iostream>

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
    PerlinNoise2D pn(189);
    MapManager mapM;
    LSystem lsystem;
    int imgWidth = 1000;
    int imgHeight = 1000;
    sf::Image img;
    img.create(imgWidth, imgHeight);

    int octaves = 8;
    int blendLvl = 8;

    std::vector<double> noiseLevels= pn.generateNoise(pn, octaves, noiseLevels, imgWidth, imgHeight); //Generate the perlin noise
    noiseLevels = pn.blendNoise(noiseLevels,blendLvl,imgHeight,imgWidth); //Blend the perlin noise to look more uniform

    noiseLevels = mapVector(noiseLevels,1,2,0,255);
    mapM.colorImg(img, imgWidth, imgHeight, noiseLevels);

    sf::Texture noise;
    noise.loadFromImage(img);

    sf::Sprite sprite;
    sprite.setTexture(noise);

    

    int chunkWidth = 200, chunkHeight=200;
    std::vector<double>chunkAvHeight= mapM.calcChunkAvHeight(chunkWidth,chunkHeight, noiseLevels,imgWidth,imgHeight);//Calculate the average height of every chunk
 
    
    int bestValueID;
    double bestValue = DBL_MAX;
    
    for (int chunkID = 0;chunkID<chunkAvHeight.size();chunkID++)
    {
        int scalingFactor = 1;
        if (chunkAvHeight[chunkID] < 80)
            scalingFactor = 100;
        if (chunkAvHeight[chunkID] >= 80 && chunkAvHeight[chunkID] <= 85)
            scalingFactor = 5;
        if (chunkAvHeight[chunkID] >= 85 && chunkAvHeight[chunkID] <= 110)
        {
            scalingFactor = 2;
        }
        if (chunkAvHeight[chunkID] > 150)
            scalingFactor = 10;
        
        

        if (bestValue > chunkAvHeight[chunkID]*scalingFactor)
        {
            bestValue = chunkAvHeight[chunkID] * scalingFactor;
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

    lsystem.applyRules(7);

  

   
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
