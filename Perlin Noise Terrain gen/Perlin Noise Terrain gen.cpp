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

void GenerateAndDisplayNoise(sf::Image img,PerlinNoise2D* pn,MapManager &mapM,int octaves,int blendLvl, sf::RectangleShape &chunkOutline,sf::Texture &noise,sf::Sprite &sprite,int &bestValueID)
{
    std::vector<double> noiseLevels = pn->generateNoise(*pn, octaves, noiseLevels); //Generate the perlin noise
    noiseLevels = pn->blendNoise(noiseLevels, blendLvl); //Blend the perlin noise to look more uniform

    noiseLevels = mapVector(noiseLevels, 1, 2, 0, 255);
    pn->noiseValues = noiseLevels;
    mapM.colorImg(img, noiseLevels);

   
    noise.loadFromImage(img);

   
    sprite.setTexture(noise);




    std::vector<double>chunkAvHeight = mapM.calcChunkAvHeight(noiseLevels);//Calculate the average height of every chunk


    bestValueID = mapM.getBestChunkID(chunkAvHeight);


    std::cout << "Size of the array of chunks: " << chunkAvHeight.size() << '\n';
    std::cout << "ID of the best chunks: " << bestValueID << '\n';
    std::cout << "Chuck start point X: " << mapM.findChunkX(bestValueID) << " Y: " << mapM.findChunkY(bestValueID) << '\n';

    

    chunkOutline.setPosition(mapM.findChunkStartP(bestValueID));

    
}


int main()
{
    
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain");
    ImGui::SFML::Init(window);

    
    int seed = 86;
    PerlinNoise2D* pn= new PerlinNoise2D(&seed); //good ones: 86
    MapManager mapM(pn);
   
    sf::Image img;
    img.create(imgWidth, imgHeight);

    int octaves = 8;
    int blendLvl = 8;
    int bestValueID=0;

    int noRules ;

    bool hasGeneratedNoise = false;
    bool showSelectedChunk = false;

    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth, chunkHeight));
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setOutlineColor(sf::Color::Transparent);
    chunkOutline.setOutlineThickness(10);

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
        if (ImGui::Button("Generate ALL"))
        {
            GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline, noise, sprite, bestValueID);
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
            }
            if (hasGeneratedNoise == false)
                hasGeneratedNoise = true;

            roads = new RoadManager(bestValueID, &window, 1, pn);
            roads->applyRules(noRules);
           
          
        }
        ImGui::SameLine();
        if (ImGui::Button("Generate Noise"))
        {
            if (pn != NULL)
            {
                delete pn;
                pn = NULL;
            }

            pn = new PerlinNoise2D(&seed);
            GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline, noise, sprite, bestValueID);

            if (hasGeneratedNoise == false)
                hasGeneratedNoise = true;

         
        }
        ImGui::SameLine();
        if (ImGui::Button("Generate Streets"))
        {
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
                
            }
            if (hasGeneratedNoise == true)
            {
                roads = new RoadManager(bestValueID, &window, 1, pn);
                roads->applyRules(noRules);
            }

           
        }
        ImGui::Text("Noise Generation parameters:");
        ImGui::InputInt("Noise seed", &seed);
        if (ImGui::InputInt("Noise octaves", &octaves))
        {
            if (octaves > 9)
                octaves = 9;
            if (octaves < 1)
                octaves = 1;
        }
        if (ImGui::InputInt("Noise blending level", &blendLvl))
        {
            if (blendLvl < 1)
                blendLvl = 1;
        }
        if (ImGui::Checkbox("Show selected chunk", &showSelectedChunk))
        {
            if (showSelectedChunk == true)
            {
                chunkOutline.setOutlineColor(sf::Color::Black);
            }
            else
            {
                chunkOutline.setOutlineColor(sf::Color::Transparent);
            }
        }
        ImGui::Text("Street Generation parameters:");

        if (ImGui::InputInt("Noise octaves", &noRules))
        {

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
