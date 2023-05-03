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

void DiscoverBestChunk(MapManager& mapM,vector<double>&noiseLevels, int& bestValueID, sf::RectangleShape& chunkOutline)
{
    std::vector<double>chunkAvHeight = mapM.calcChunkAvHeight(noiseLevels);//Calculate the average height of every chunk


    bestValueID = mapM.getBestChunkID(chunkAvHeight);


    std::cout << "Size of the array of chunks: " << chunkAvHeight.size() << '\n';
    std::cout << "ID of the best chunks: " << bestValueID << '\n';
    std::cout << "Chuck start point X: " << mapM.findChunkX(bestValueID) << " Y: " << mapM.findChunkY(bestValueID) << '\n';



    chunkOutline.setPosition(mapM.findChunkStartP(bestValueID));
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




    DiscoverBestChunk(mapM, noiseLevels, bestValueID, chunkOutline);

    
}


int main()
{
    
    srand(time(NULL));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Perlin Noise Terrain", sf::Style::Default, settings);
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
    vector<double>oldValues;

    //Street Network values
    int rulesNumber=5 ;
    int roadsType = 1;
    bool viewPlots = false;
    bool viewNodes = false;

    //Buildings values
    int grammarIterations = 4;

  

   
    bool isInChunkView = false;



    sf::RectangleShape chunkOutline(sf::Vector2f(chunkWidth, chunkHeight));
    chunkOutline.setFillColor(sf::Color::Transparent);
    chunkOutline.setOutlineColor(sf::Color::Transparent);
    chunkOutline.setOutlineThickness(10);

    sf::View fullscreen(sf::Vector2f(750, 750), sf::Vector2f(1500, 1500));

    sf::View chunkView;
   chunkView.setViewport(sf::FloatRect(0, 0,0.665, 0.665));
    


    sf::Texture noise;
    sf::Sprite sprite;

   // GenerateAndDisplayNoise(img, pn, mapM, octaves, blendLvl, chunkOutline,noise, sprite,bestValueID);
   
    RoadManager* roads=NULL;

    City* city = NULL;

    sf::Clock deltaClock;




    /////////////////////
    //Editing variables//
    /////////////////////

    //Noise levels
    bool enableNoiseEdit = false;
    string noiseEditName = "Edit noise levels";

    sf::RectangleShape rect(sf::Vector2f(imgWidth, imgHeight));
    sf::RectangleShape brush(sf::Vector2f(5,5));
    rect.setFillColor(sf::Color(255, 255, 255, 60));

    vector<sf::RectangleShape> editorDrawables;
    editorDrawables.push_back(rect);
    editorDrawables.push_back(brush);
    brush.setSize(sf::Vector2f(10, 10));
    sf::Mouse mouseRef;
    bool mouseLeftPress = false;
    bool mouseRightPress = false;
    int brushSize = 0;


    //Streets Editing
    bool enableStreetsEdit = false;
    string streetEditName = "Edit streets";
    Node* node1=NULL;
    Node* node2=NULL;
  

   
    while (window.isOpen())
    {
       
        editorDrawables[1].setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        editorDrawables[1].setSize(sf::Vector2f(brushSize + 1, brushSize + 1));


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
                           mouseLeftPress = true;
                      
                       }
                       else
                           if (event.mouseButton.button == sf::Mouse::Right)
                           {
                               mouseRightPress = true;
                           
                           }
                   }

               }


                   if (event.type == sf::Event::MouseButtonReleased)
                   {
                       if (event.mouseButton.button == sf::Mouse::Left)
                       {
                           mouseLeftPress = false;
                           pn->noiseValues = pn->blendNoiseNew(pn->noiseValues, oldValues, blendLvl);
                           mapM.colorImg(img, pn->noiseValues);


                           noise.loadFromImage(img);


                           sprite.setTexture(noise);
                           oldValues = pn->noiseValues;
                       }

                       if (event.mouseButton.button == sf::Mouse::Right)
                       {
                           mouseRightPress = false;
                           pn->noiseValues = pn->blendNoiseNew(pn->noiseValues, oldValues, blendLvl);
                           mapM.colorImg(img, pn->noiseValues);


                           noise.loadFromImage(img);


                           sprite.setTexture(noise);
                           oldValues = pn->noiseValues;
                       }


                   }
               
           }
           else
           {
               if (mouseLeftPress == true)
               {
                   mouseLeftPress = false;
                
               }
               if (mouseRightPress == true)
               {
                   mouseRightPress = false;

               }
           }

           if (enableStreetsEdit == true)
           {
               if (roads != NULL&&roads->RoadNetwork->AllNodes.size()>0)
               {
                   if (event.type == sf::Event::MouseButtonPressed)
                   {
                       if (event.mouseButton.button == sf::Mouse::Left)
                       {


                           sf::Vector2i position = sf::Mouse::getPosition(window);
                           sf::Vector2f positionView = window.mapPixelToCoords(position);
                          
                           if (positionView.x < imgWidth && positionView.y < imgHeight)
                           {
                               Node* potentialNode = roads->RoadNetwork->findNearbyNode(positionView, 10);
                               if (potentialNode == NULL)
                               {
                                   if (node1 == NULL)
                                   {
                                       node1 = new Node();
                                       node1->endPos = positionView;
                                       roads->RoadNetwork->AllNodes.push_back(node1);
                                   }
                                   else
                                       if (node2 == NULL)
                                       {
                                           node2 = new Node();
                                           node2->endPos = positionView;
                                           roads->RoadNetwork->connectNodes(node1, node2);
                                           roads->RoadNetwork->AllNodes.push_back(node2);
                                           node1 = NULL;
                                           node2 = NULL;
                                           roads->plots.clear();
                                           roads->generatePlots(roads->RoadNetwork->AllNodes, NULL, NULL, 0);

                                       }
                               }
                               else
                               {
                                   if (node1 == NULL)
                                   {
                                       node1 = potentialNode;
                                   }
                                   else
                                       if (node2 == NULL && potentialNode != node1)
                                       {
                                           node2 = potentialNode;
                                           roads->RoadNetwork->connectNodes(node1, node2);
                                           node1 = NULL;
                                           node2 = NULL;
                                           roads->plots.clear();
                                           roads->generatePlots(roads->RoadNetwork->AllNodes, NULL, NULL, 0);
                                       }
                               }
                           }
                       }

                       if (event.mouseButton.button == sf::Mouse::Right)
                       {
                    
                           int detectionRange = 7;
                           bool lineFound = false;
                           sf::Vector2i position = sf::Mouse::getPosition(window);
                          
                           vector<Node*> nodes;
                           for (int p = -detectionRange; p <= detectionRange && lineFound==false; p++)
                           {
                               for (int q = -detectionRange; q <= detectionRange && lineFound == false; q++)
                               {
                                   sf::Vector2i positionNew (position.x+p,position.y+q);
                                   sf::Vector2f positionView = window.mapPixelToCoords(positionNew);

                                   nodes = roads->RoadNetwork->findNodesOfLine(positionView);

                                   if (nodes.size() > 0)
                                   {
                                       roads->RoadNetwork->disconnectNodes(nodes[0], nodes[1]);
                                       roads->plots.clear();
                                       roads->generatePlots(roads->RoadNetwork->AllNodes, NULL, NULL, 0);
                                       lineFound = true;
                                   }
                               }
                           }
                           if (lineFound == false)
                           {
                               sf::Vector2f potentialNodePos = window.mapPixelToCoords(position);
                               Node* potentialNode = roads->RoadNetwork->findNearbyNode(potentialNodePos, 7);
                               if (potentialNode != NULL)
                               {
                                 
                                   roads->RoadNetwork->removeNode(potentialNode);
                                   node1 = NULL;
                                   node2 = NULL;
                                   roads->plots.clear();
                                   roads->generatePlots(roads->RoadNetwork->AllNodes, NULL, NULL, 0);
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

        if (mouseLeftPress == true)
        {
            cout << "Left clicked\n";
            if (pn->noiseValues.size() > sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y)
            {
                
                {
                   // cout << "Trying to draw at:" << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << " " << "vector ID: " << sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y << " new value: " << pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] << '\n';
                
                    for (int p = -brushSize; p <= brushSize; p++)
                    {
                        for (int q = -brushSize; q <= brushSize; q++)
                        {

                            sf::Vector2i positionMouse = sf::Mouse::getPosition(window);
                            sf::Vector2i positionView(window.mapPixelToCoords(positionMouse).x, window.mapPixelToCoords(positionMouse).y);


                            int position = (positionView.x + p)* imgWidth + positionView.y + q;


                            if (position < pn->noiseValues.size()&&0<=position)
                            {
                                if (pn->noiseValues[position] + 10 < 256)
                                {
                                    pn->noiseValues[position] += 10;
                                }
                            }
                        }
                    }

                  //  cout << "Drew at:" << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << " " << "vector ID: " << sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y << " new value: " << pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] << '\n';




                    mapM.colorImg(img, pn->noiseValues);


                    noise.loadFromImage(img);


                    sprite.setTexture(noise);

             

                    DiscoverBestChunk(mapM, pn->noiseValues, bestValueID, chunkOutline);

                }
            }
        }
        else
            if(mouseRightPress==true)
            {
                cout << "Right clicked\n";
                if (pn->noiseValues.size() > sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y)//Check if you are clicking on the noise texture
                {
                    if (pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] - 10 > 0)
                    {
                        for (int p = -brushSize; p <= brushSize; p++)
                        {
                            for (int q = -brushSize; q <= brushSize; q++)
                            {
                                sf::Vector2i positionMouse = sf::Mouse::getPosition(window);
                                sf::Vector2i positionView(window.mapPixelToCoords(positionMouse).x, window.mapPixelToCoords(positionMouse).y);


                                int position = (positionView.x + p) * imgWidth + positionView.y + q;

                                if (position < pn->noiseValues.size() && 0 <= position)
                                {
                                    if (pn->noiseValues[position] - 10 < 256)
                                    {
                                        pn->noiseValues[position] -= 10;
                                    }
                                }
                            }
                        }

                        //  cout << "Drew at:" << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << " " << "vector ID: " << sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y << " new value: " << pn->noiseValues[sf::Mouse::getPosition(window).x * imgWidth + sf::Mouse::getPosition(window).y] << '\n';

                        

                        mapM.colorImg(img, pn->noiseValues);


                        noise.loadFromImage(img);


                        sprite.setTexture(noise);

                        DiscoverBestChunk(mapM, pn->noiseValues, bestValueID, chunkOutline);
                    }
                }
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
            oldValues = pn->noiseValues;
          
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

            oldValues = pn->noiseValues;
         
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
            if (roads != NULL)
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
        if (ImGui::InputInt("Brush Size", &brushSize))
        {
            if (brushSize < 0)
            {
                brushSize = 0;
            }

            if (brushSize > 10)
            {
                brushSize = 10;
            }
        }


        ImGui::Text("Edit street network");
        if (ImGui::Button(streetEditName.c_str()))
        {
            if (roads != NULL)
            {
                if (enableStreetsEdit == true)
                {
                    enableStreetsEdit = false;
                    streetEditName = "Edit streets";
                    if (viewNodes == true)
                    {
                        viewNodes = false;
                        roads->showNodes = false;
                    }
                }
                else
                {
                    if (city != NULL)
                    {
                        delete city;
                        city = NULL;
                    }
                    if (viewNodes == false)
                    {
                        viewNodes = true;
                        roads->showNodes = true;
                    }
                    enableStreetsEdit = true;
                    streetEditName = "Exit editing";

                }
            }
        }



        ImGui::Text("Edit Buildings");




        ImGui::End();

       



        window.clear(sf::Color::Black);

        window.draw(sprite);
        window.draw(chunkOutline);
       
       
        roads->drawRoads();
       // city->drawCenters();
        city->display();
        if (enableNoiseEdit == true)
        {
            for (auto &element : editorDrawables)
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
