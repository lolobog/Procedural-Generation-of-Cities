#include "RoadManager.h"
#include "imgui.h"
#include "imgui-SFML.h"



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

void GenerateAndDisplayNoise(sf::Image img,PerlinNoise2D &pn,MapManager &mapM,int octaves,int blendLvl, sf::RectangleShape &chunkOutline,sf::Texture &noise,sf::Sprite &sprite,int &bestValueID)
{
    std::vector<double> noiseLevels = pn.generateNoise(pn, octaves, noiseLevels); //Generate the perlin noise
    noiseLevels = pn.blendNoise(noiseLevels, blendLvl); //Blend the perlin noise to look more uniform

    noiseLevels = mapVector(noiseLevels, 1, 2, 0, 255);
    pn.noiseValues = noiseLevels;
    mapM.colorImg(img, noiseLevels);

   
    noise.loadFromImage(img);

   
    sprite.setTexture(noise);




    std::vector<double>chunkAvHeight = mapM.calcChunkAvHeight(noiseLevels);//Calculate the average height of every chunk


    bestValueID = mapM.getBestChunkID(chunkAvHeight);


    std::cout << "Size of the array of chunks: " << chunkAvHeight.size() << '\n';
    std::cout << "ID of the best chunks: " << bestValueID << '\n';
    std::cout << "Chuck start point X: " << mapM.findChunkX(bestValueID) << " Y: " << mapM.findChunkY(bestValueID) << '\n';

    
    chunkOutline.setOutlineColor(sf::Color::Black);
    chunkOutline.setOutlineThickness(10);
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setPosition(mapM.findChunkStartP(bestValueID));

    
}


int main()
{
    
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    ImGui::SFML::Init(window);


    PerlinNoise2D pn(90); //good ones: 86
    MapManager mapM(&pn);
   
    sf::Image img;
    img.create(imgWidth, imgHeight);

    int octaves = 8;
    int blendLvl = 8;
    int bestValueID=0;

    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth, chunkHeight));
    sf::Texture noise;
    sf::Sprite sprite;

   // GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline,noise, sprite,bestValueID);
   
    RoadManager* roads=NULL;


    sf::Clock deltaClock;
  

    
   
  

   
    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
           ImGui::SFML::ProcessEvent(window,event);

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                   
                    if (octaves == 9)
                    {
                        octaves = 1;
                        std::cout << "\nOctaves back to 1";
                    }
                    else
                    {

                        octaves++;
                        std::cout << "\nIncreased octaves: ";
                        std::cout << octaves;
                    }

                     
                }

                

                if (event.key.code == sf::Keyboard::Z)
                {
                   
                
                }

                if (event.key.code == sf::Keyboard::W)
                {
                    std::cout << "\nIncreased octaves: ";
                    
                }
                if (event.key.code == sf::Keyboard::S)
                {
                  
                }

            }


            if (event.type == sf::Event::Closed)
                window.close();
        }
      
      
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Window");
        ImGui::Text("Generate noise and streets:");
        if (ImGui::Button("Generate"))
        {
            GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline, noise, sprite, bestValueID);
            std::cout << bestValueID;
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
            }

            roads = new RoadManager(bestValueID, &window, 1, &pn);
            roads->applyRules(2);
           
            std::cout << "Reset seed\n";
        }
        if (ImGui::Button("Generate Streets"))
        {
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
                roads = new RoadManager(bestValueID, &window, 1, &pn);
                roads->applyRules(2);
            }

           
        }
        ImGui::End();

        ImGui::ShowDemoWindow();



        window.clear(sf::Color::Black);

        window.draw(sprite);
        window.draw(chunkOutline);
        ImGui::SFML::Render(window);

        roads->drawRoads();
       

        window.display();
       // sf::sleep(sf::milliseconds(10));
    }
   ImGui::SFML::Shutdown();
    return 0;

}
