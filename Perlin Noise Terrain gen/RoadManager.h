#pragma once
#include "LSystem.h"
#include "MapManager.h"

class Node
{
public:
	Node* parent;
	sf::Vector2f endPos;
	char nodeType;
	std::vector<Node*>nodeLinks;

	bool rulesApplied = false;

};

class NodeTree
{
public:
	
	NodeTree(){ };
	~NodeTree() { };

	Node root;
	std::vector<Node*>AllNodes;

	void setRoot(sf::Vector2f Pos,char nodeInfo)
	{
		root.endPos = Pos;
		root.nodeType = nodeInfo;
		AllNodes.push_back(&root);
	}

	void newLink(Node* targetNode, sf::Vector2f Pos,char nodeInfo)
	{
		Node* newNode= new Node();
		newNode->parent = targetNode;
		newNode->endPos = Pos;
		newNode->nodeType = nodeInfo;

		targetNode->nodeLinks.push_back(newNode);
		AllNodes.push_back(newNode);
	}

	bool isIntersecting(sf::Vector2f point)
	{
		for (auto element : AllNodes)
		{
			if (element->parent != NULL)
			{
				if (element->parent->endPos.x == point.x)
				{
					if ((element->parent->endPos.y <= point.y && point.y >= element->endPos.y) || (element->parent->endPos.y >= point.y && point.y <= element->endPos.y))
					{
						return true;
					}
				}

				if (element->parent->endPos.y == point.y)
				{
					if ((element->parent->endPos.x <= point.x && point.x >= element->endPos.x) || (element->parent->endPos.x >= point.x && point.x <= element->endPos.x))
					{
						return true;
					}
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
	
	RoadManager(int chunkID,sf::RenderWindow* window);
	~RoadManager();


	NodeTree *RoadNetwork=new NodeTree;

	
	
	
	
	void applyRules(int iterations);
	void drawRoads();

private:
	int roadLength = 20;
	int currentChunk;
	sf::RenderWindow* refWindow;
};