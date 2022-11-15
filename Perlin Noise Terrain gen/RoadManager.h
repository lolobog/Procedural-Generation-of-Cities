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
		Node* newNode= new Node();
		newNode->parent = targetNode;
		newNode->endPos = Pos;
		newNode->nodeType = nodeInfo;

		targetNode->nodeLinks.push_back(newNode);
		AllNodes.push_back(newNode);
		CurrentNodes.push_back(newNode);

	}

	bool isIntersecting(sf::Vector2f point)
	{
		for (auto element : AllNodes)
		{
			if (element->parent != NULL)
			{
				float diffx = abs(element->parent->endPos.x - point.x);

				if (diffx < std::numeric_limits<float>::epsilon())
				{
					if ((element->parent->endPos.y <= point.y && point.y >= element->endPos.y) || (element->parent->endPos.y >= point.y && point.y <= element->endPos.y))
					{
						return true;
					}
				}

				float diffy = abs(element->parent->endPos.y - point.y);

				if (diffy < std::numeric_limits<float>::epsilon())
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
	int random(int low, int high);
private:
	int roadLength = 25;
	int currentChunk;
	sf::RenderWindow* refWindow;
};