#include "BuildingsManager.h"
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
    ////////////////////////
    //Generation variables//
    ////////////////////////

    //Noise values
    int octaves = 8;
    int blendLvl = 8;
    int bestValueID=0;
    bool hasGeneratedNoise = false;
    bool showSelectedChunk = false;

    //Street Network values
    int rulesNumber=5 ;
    int roadsType = 1;
    bool viewPlots = false;
    bool viewNodes = false;

    //Buildings values
    int grammarIterations = 4;

    /////////////////////
    //Editing variables//
    /////////////////////

    //Noise levels
    bool enableNoiseEdit = false;
    string noiseEditName = "Edit noise levels";

   
    bool isInChunkView = false;



    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth, chunkHeight));
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setOutlineColor(sf::Color::Transparent);
    chunkOutline.setOutlineThickness(10);

    sf::View fullscreen(sf::Vector2f(750, 750), sf::Vector2f(1500, 1500));

    sf::View chunkView;
    


    sf::Texture noise;
    sf::Sprite sprite;

   // GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline,noise, sprite,bestValueID);
   
    RoadManager* roads=NULL;

    City* city = NULL;

    sf::Clock deltaClock;

    sf::RectangleShape rect(sf::Vector2f(imgWidth, imgHeight));
    sf::RectangleShape brush(sf::Vector2f(5,5));
    rect.setFillColor(sf::Color(255, 255, 255, 60));

    vector<sf::RectangleShape> editorDrawables;
    editorDrawables.push_back(rect);
    editorDrawables.push_back(brush);
  
    sf::Mouse mouseRef;
    
    
   
  

   
    while (window.isOpen())
    {
        
        sf::Event event;
        while (window.pollEvent(event))
        {
           ImGui::SFML::ProcessEvent(window,event);


           if (enableNoiseEdit == true)
           {
               if (event.type == sf::Event::MouseButtonPressed)
               {
                   if (pn != NULL)
                   {
                       if (event.mouseButton.button == sf::Mouse::Left)
                       {
                           cout << "Left clicked\n";
                           if (pn->noiseValues.size() > sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y)
                           {
                               if (pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] + 10 < 256)
                               {
                                   cout << "Trying to draw at:" << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y<<" "<<"vector ID: "<< sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y<<" new value: "<< pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y]<<'\n';
                                   pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] += 10;
                                   pn->noiseValues[(sf::Mouse::getPosition(window).x+1) * imgWidth + sf::Mouse::getPosition(window).y] += 10;
                                   pn->noiseValues[(sf::Mouse::getPosition(window).x - 1) * imgWidth + sf::Mouse::getPosition(window).y] += 10;
                                   pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y+1] += 10;
                                   pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y-1] += 10;
                                   pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y+2] += 10;
                                   pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y-2] += 10;
                                   pn->noiseValues[(sf::Mouse::getPosition(window).x+2) * imgWidth + sf::Mouse::getPosition(window).y] += 10;
                                   pn->noiseValues[(sf::Mouse::getPosition(window).x-2) * imgWidth + sf::Mouse::getPosition(window).y] += 10;

                                   cout << "Drew at:" << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << " " << "vector ID: " << sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y << " new value: " << pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] << '\n';



                                   mapM.colorImg(img, pn->noiseValues);


                                   noise.loadFromImage(img);


                                   sprite.setTexture(noise);
                               }
                           }
                       }
                       else
                           if (event.mouseButton.button == sf::Mouse::Right)
                           {
                               cout << "Right clicked\n";
                               if (pn->noiseValues.size() > sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y)//Check if you are clicking on the noise texture
                               {
                                   if (pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] - 10 > 0)
                                   {
                                       pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] -= 10;




                                       mapM.colorImg(img, pn->noiseValues);


                                       noise.loadFromImage(img);


                                       sprite.setTexture(noise);
                                   }
                               }
                           }
                   }
               }
           }


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

        ImGui::Begin("Generation Window");
        ImGui::Text("Generate:");
        if (ImGui::Button("Generate ALL"))
        {
            if (pn != NULL)
            {
                delete pn;
                pn = NULL;
            }
            pn = new PerlinNoise2D(&seed);
            GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline, noise, sprite, bestValueID);
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
            }
            if (hasGeneratedNoise == false)
                hasGeneratedNoise = true;

            roads = new RoadManager(bestValueID, &window, &roadsType, pn);
            roads->applyRules(rulesNumber);

            if (city != NULL)
            {
                delete city;
                city = NULL;
            }
            city = new City(roads, grammarIterations);
            city->extractPlotLimits();
            city->findPlotCenters();
            city->create();
           
          
        }
       // ImGui::SameLine();
        if (ImGui::Button("Generate Noise"))
        {
            if (pn != NULL)
            {
                delete pn;
                pn = NULL;
            }

            if (roads != NULL)
            {
                delete roads;
                roads = NULL;
            }
            if (city != NULL)
            {
                delete city;
                city = NULL;
            }

            pn = new PerlinNoise2D(&seed);
            GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline, noise, sprite, bestValueID);

            if (hasGeneratedNoise == false)
                hasGeneratedNoise = true;

         
        }
        //ImGui::SameLine();
        if (ImGui::Button("Generate Streets"))
        {
            if (roads != NULL)
            {
                delete roads;
                roads = NULL;               
            }
            if (city != NULL)
            {
                delete city;
                city = NULL;
            }
            if (hasGeneratedNoise == true)
            {
                roads = new RoadManager(bestValueID, &window, &roadsType, pn);
                roads->showNodes = viewNodes;
                roads->showPlots = viewPlots;
                roads->applyRules(rulesNumber);


           
            }

           
           
        }
        if (ImGui::Button("Generate Buildings"))
        {
            if (city != NULL)
            {
                delete city;
                city = NULL;
            }
            city = new City(roads, grammarIterations);
            city->extractPlotLimits();
            city->findPlotCenters();
            city->create();
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
        if (ImGui::Checkbox("Zoom In", &isInChunkView))
        {

            if (isInChunkView == false)
            {
                window.setView(fullscreen);
            }
            else
            {
                chunkView.setCenter(sf::Vector2f(chunkOutline.getPosition().x+chunkHeight/2, chunkOutline.getPosition().y + chunkHeight / 2));
                chunkView.setSize(chunkOutline.getSize());
                window.setView(chunkView);
            }

        }
        ImGui::Text("Street Generation parameters:");

        if (ImGui::InputInt("Rules", &rulesNumber))
        {
            if (rulesNumber < 1)
            {
                rulesNumber = 1;
            }

            if (rulesNumber > 8)
            {
                rulesNumber = 5;
            }
        }

        if (ImGui::InputInt("Roads Type: 1.Grid Style; 2.Mixed", &roadsType))
        {
            if (roadsType < 1)
            {
                roadsType = 1;
            }
            if (roadsType > 2)
            {
                roadsType = 2;
            }
        }

        if (ImGui::Checkbox("View Nodes", &viewNodes))
        {

            if (viewNodes == true)
            {
                roads->showNodes = true;
            }
            else
            {
                roads->showNodes = false;
            }

        }

        

        if (ImGui::Checkbox("View Plots", &viewPlots))
        {
            if (roads != NULL)
            {
                if (viewPlots == true)
                {
                    roads->showPlots = true;
                }
                else
                {
                    roads->showPlots = false;
                }
            }

        }

        ImGui::Text("Building Generation parameters:");

       

        if (ImGui::InputInt("Complexity", &grammarIterations))
        {
            if (grammarIterations < 1)
            {
                grammarIterations = 1;
            }

            if (grammarIterations > 4)
            {
                grammarIterations = 4;
            }
        }

  

       
        ImGui::End();

      



        ImGui::Begin("Editing Window");


        ImGui::Text("Edit noise");
        if (ImGui::Button(noiseEditName.c_str()))
        {
            if (enableNoiseEdit == true)
            {
                enableNoiseEdit = false;
                noiseEditName = "Edit noise levels";
            }
            else
            {
                if (roads != NULL)
                {
                    delete roads;
                    roads = NULL;
                }
                if (city != NULL)
                {
                    delete city;
                    city = NULL;
                }
                enableNoiseEdit = true;
                noiseEditName = "Exit editing";
               
            }
        }


        ImGui::Text("Edit street network");



        ImGui::Text("Edit Buildings");




        ImGui::End();

        ImGui::ShowDemoWindow();



        window.clear(sf::Color::Black);

        window.draw(sprite);
        window.draw(chunkOutline);
       
       
        roads->drawRoads();
        city->drawCenters();
        city->display();
        if (enableNoiseEdit == true)
        {
            for (auto element : editorDrawables)
            {
                window.draw(element);
            }
           // window.draw(rect);
        }
       
        ImGui::SFML::Render(window);
        window.display();
       // sf::sleep(sf::milliseconds(10));
    }
   ImGui::SFML::Shutdown();
    return 0;

}
