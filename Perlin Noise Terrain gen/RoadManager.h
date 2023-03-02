#pragma once
#include "LSystem.h"
#include "MapManager.h"

#include <limits>

class Node
{
public:
	Node* parent;
	sf::Vector2f endPos;
	char nodeType;
	std::vector<Node*>nodeLinks;
	bool rulesApplied = false;
private:

};

class NodeTree
{
public:
	
	NodeTree(){ };
	~NodeTree() { };

	Node root;
	std::vector<Node*>AllNodes;
	std::vector<Node*>CurrentNodes;
	void setRoot(sf::Vector2f Pos,char nodeInfo)
	{
		root.endPos = Pos;
		root.nodeType = nodeInfo;
		AllNodes.push_back(&root);
		CurrentNodes.push_back(&root);
	}

	void newLink(Node* targetNode, sf::Vector2f Pos,char nodeInfo)
	{
		
		Node* newNode = new Node();
		newNode->parent = targetNode;
		newNode->endPos = Pos;
		newNode->nodeType = nodeInfo;

		CurrentNodes.push_back(newNode);
		targetNode->nodeLinks.push_back(newNode);
		AllNodes.push_back(newNode);
		

	}

	bool isIntersecting(sf::Vector2f point)
	{
		for (auto element : AllNodes)
		{
			if (element->parent != NULL)
			{				
				sf::Vector2f point1(element->parent->endPos.x, element->parent->endPos.y);
				sf::Vector2f point2(element->endPos.x, element->endPos.y);

				float dxc = point.x - point1.x;
				float dyc = point.y - point1.y;

				float dxl = point2.x - point1.x;
				float dyl = point2.y - point1.y;

				float cross = dxc * dyl - dyc * dxl;

				if (cross != 0)
				{
					return false;
				}
				else
				{
					if (abs(dxl) >= abs(dyl))
						return dxl > 0 ?
						point1.x <= point.x && point.x <= point2.x :
						point2.x <= point.x && point.x <= point1.x;
					else
						return dyl > 0 ?
						point1.y <= point.y && point.y <= point2.y :
						point2.y <= point.y && point.y <= point1.y;
				}

				

			
			}
			

		}

		return false;
	}

private:
	
};







class RoadManager :public LSystem,public MapManager
{
public:
	
	RoadManager(int chunkID,sf::RenderWindow* window,int roadsType,PerlinNoise2D* pn);
	~RoadManager();


	NodeTree *RoadNetwork=new NodeTree;
	std::vector<sf::Vector2f> plots;
	
	
	
	
	void applyRules(int iterations);
	void drawRoads();
	int random(int low, int high);
	bool isPositionPlotted(sf::Vector2f position)
	{
		for (int i = 0; i < plots.size(); i++)
		{
			if (plots[i] == position)
			{
				return true;
			}
		}

		return false;
	}


private:
	int roadLength = 20;
	int currentChunk;
	int rType = 1;
	sf::RenderWindow* refWindow;
	
};