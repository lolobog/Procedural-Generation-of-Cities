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
	int nodeID;
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
	std::vector<Node*>IterationNodes;
	void setRoot(sf::Vector2f Pos,char nodeInfo)
	{
		root.endPos = Pos;
		root.nodeType = nodeInfo;
		root.nodeID = AllNodes.size();
		AllNodes.push_back(&root);
		CurrentNodes.push_back(&root);
	}

	void newLink(Node* targetNode, sf::Vector2f Pos,char nodeInfo)
	{
		
		Node* newNode = new Node();
		newNode->parent = targetNode;
		newNode->endPos = Pos;
		newNode->nodeType = nodeInfo;
		newNode->nodeID = AllNodes.size();

		IterationNodes.push_back(newNode);
		targetNode->nodeLinks.push_back(newNode);
		AllNodes.push_back(newNode);
		

	}

	void connectNodes(Node* node1, Node* node2)
	{
		node1->nodeLinks.push_back(node2);
		node2->nodeLinks.push_back(node1);
	}

	float distance(int x1, int y1, int x2, int y2)
	{
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
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

	bool isOverlappingNode(sf::Vector2f point)
	{
		for (auto element : AllNodes)
		{
			if (element->endPos == point)
			{
				return true;
			}
		}

		return false;
	}

	Node* getNode(sf::Vector2f point)
	{
		for (auto element : AllNodes)
		{
			if (element->endPos == point)
			{
				return element;
			}
		}

		cout << "No nodes with given coordinates";
		return NULL;
	}

private:
	
};







class RoadManager :public LSystem,public MapManager
{
public:
	
	RoadManager(int chunkID,sf::RenderWindow* window,int* roadsType,PerlinNoise2D* pn);
	~RoadManager();


	NodeTree *RoadNetwork=new NodeTree;
	std::vector<sf::Vector2f> plots;
	
	
	
	
	void GenerateRoadChunk(Node* element, float plotGenerationChance, char dir, float length);

	void applyRules(int iterations);
	bool checkRules(sf::Vector2f pos);
	void drawRoads();
	void generatePlots(std::vector<Node*>AllNodes,Node* previousNode,Node* targetNode ,int depth);
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
	int* typeOfRoads;
	int roadLength = 20;
	int currentChunk;
	int rType = 1;
	sf::RenderWindow* refWindow;
	
};