#pragma once
#include "LSystem.h"
#include "MapManager.h"

class Node
{
public:
	Node* parent;
	sf::Vector2f endPos;
	std::vector<Node*>nodeLinks;
};

class NodeTree
{
public:
	
	NodeTree(){ };
	~NodeTree() { };

	Node root;
	std::vector<Node*>AllNodes;

	void setRoot(sf::Vector2f Pos)
	{
		root.endPos = Pos;
		AllNodes.push_back(&root);
	}

	void newLink(Node* targetNode, sf::Vector2f Pos)
	{
		Node newNode;
		newNode.parent = targetNode;
		newNode.endPos = Pos;

		targetNode->nodeLinks.push_back(&newNode);
		AllNodes.push_back(&newNode);
	}

private:
	
};



class RoadManager :public LSystem,public MapManager
{
public:
	
	RoadManager(int chunkID);
	~RoadManager();


	NodeTree *RoadNetwork=new NodeTree;

	
	
	
	
	void applyRules(int iterations);
private:
	int roadLength = 10;
};