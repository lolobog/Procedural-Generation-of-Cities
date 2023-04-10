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

		//cout << "Created new node: " + AllNodes.size();

		IterationNodes.push_back(newNode);
		targetNode->nodeLinks.push_back(newNode);
		AllNodes.push_back(newNode);
		

	}

	void connectNodes(Node* node1, Node* node2)
	{
		node1->nodeLinks.push_back(node2);
		node2->nodeLinks.push_back(node1);
		cout << "Connected nodes " << node1->nodeID << " TO " << node2->nodeID << '\n';
		
		
	}

	float distance(int x1, int y1, int x2, int y2)
	{
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
	}

	float distancePoints(sf::Vector2f point1,sf::Vector2f point2)
	{
		int x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
	}

	Node* findNearbyNode(sf::Vector2f pos,float radius)
	{
		for (auto element : AllNodes)
		{
			if (distancePoints(pos, element->endPos) <= radius)
			{
				return element;
			}
		}

		return NULL;
	}

	

	bool isOnLine(sf::Vector2f point)
	{
		bool status = false;
		for (auto &element : AllNodes)
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
					status= false;
				}
				else
				{
					if (abs(dxl) >= abs(dyl))
						status= dxl > 0 ?
						point1.x <= point.x && point.x <= point2.x :
						point2.x <= point.x && point.x <= point1.x;
					else
						status= dyl > 0 ?
						point1.y <= point.y && point.y <= point2.y :
						point2.y <= point.y && point.y <= point1.y;
				}

				

			
			}

			if (status == true)
			{
				return status;
			}
			

		}

		return status;
	}


	bool checkIfOnLine(sf::Vector2f point, sf::Vector2f p1, sf::Vector2f p2)
	{
		float dxc = point.x - p1.x;
		float dyc = point.y - p1.y;

		float dxl = p2.x - p1.x;
		float dyl = p2.y - p1.y;

		float cross = dxc * dyl - dyc * dxl;

		if (cross != 0)
		{
			return false;
		}
		else
		{
			if (abs(dxl) >= abs(dyl))
				return  dxl > 0 ?
				p1.x <= point.x && point.x <= p2.x :
				p2.x <= point.x && point.x <= p1.x;
			else
				return dyl > 0 ?
				p1.y <= point.y && point.y <= p2.y :
				p2.y <= point.y && point.y <= p1.y;
		}


		return false;


	}

	

	bool isIntersecting(sf::Vector2f point,sf::Vector2f parent)
	{
		bool status = false;

		for (auto& element : AllNodes)
		{
			for (auto link : element->nodeLinks)
			{
				if (checkIfOnLine(point, element->endPos, link->endPos)|| (element->parent!=NULL&&checkIfOnLine(point, element->endPos, element->parent->endPos)))
				{
					status = false;
				}
				else
				{
					{
						status = checkIntersection(point, parent, element->endPos, link->endPos);
						if (status == true)
						{
							cout << "Line formed from: " << point.x << ' ' << point.y << " AND " << parent.x << ' ' << parent.y << " is intersecting line formed from: " << element->endPos.x << ' ' << element->endPos.y << " AND " << link->endPos.x << ' ' << link->endPos.y << '\n';
							return true;

						}

					}
					if (element->parent != NULL)
					{
						status = checkIntersection(point, parent, element->endPos, element->parent->endPos);
						if (status == true)
						{
							cout << "Line formed from: " << point.x << ' ' << point.y << " AND " << parent.x << ' ' << parent.y << " is intersecting line formed from: " << element->endPos.x << ' ' << element->endPos.y << " AND " << element->parent->endPos.x << ' ' << element->parent->endPos.y << '\n';
							return true;

						}
					}
				}
			}
		}

		return status;

		
	}

	bool checkIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4)
	{
		double x1 = p1.x;
		double y1 = p1.y;
		double x2 = p2.x;
		double y2 = p2.y;
		double x3 = p3.x;
		double y3 = p3.y;
		double x4 = p4.x;
		double y4 = p4.y;

		double denominator = ((y4 - y3) * (x2 - x1)) - ((x4 - x3) * (y2 - y1));
		double numerator1 = ((x4 - x3) * (y1 - y3)) - ((y4 - y3) * (x1 - x3));
		double numerator2 = ((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3));

		// Check if the lines are parallel
		if (denominator == 0)
		{
			return false;
		}

		double r = numerator1 / denominator;
		double s = numerator2 / denominator;

		if (r < 0 || r > 1 || s < 0 || s > 1)
		{
			return false;
		}

		return true;
	}

	//This function assumes that the lines are not paralel
	sf::Vector2f getIntersection(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4)
	{
		sf::Vector2f intersection;
		// calculate the denominator
		double den = ((p2.x - p1.x) * (p4.y - p3.y)) - ((p4.x - p3.x) * (p2.y - p1.y));

		// calculate the numerator for the first line
		double num1 = ((p4.x - p3.x) * (p1.y - p3.y)) - ((p4.y - p3.y) * (p1.x - p3.x));
		// calculate the numerator for the second line
		double num2 = ((p2.x - p1.x) * (p1.y - p3.y)) - ((p2.y - p1.y) * (p1.x - p3.x));

		// calculate the parameters t1 and t2
		double t1 = num1 / den;
		double t2 = num2 / den;

		// check if the intersection point is within both line segments
		if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
		{
			// calculate and return the intersection point
			intersection.x = p1.x + (t1 * (p2.x - p1.x));
			intersection.y = p1.y + (t1 * (p2.y - p1.y));
		}
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
	vector<vector<Node*>>plots;
	vector<Node*>tempPlot;
	int plotIndex = 0;

	bool showNodes = false;
	bool showPlots = false;




	
	
	
	
	void GenerateRoadChunk(Node* element, float plotGenerationChance, char dir, float length,bool applyRules);

	void applyRules(int iterations);
	bool checkRules(sf::Vector2f pos,sf::Vector2f parent, bool applyRules);
	bool plotExsists(vector<Node*>plot);
	bool isInTempPlot(Node* node);
	void drawRoads();
	void generatePlots(std::vector<Node*>AllNodes,Node* previousNode,Node* targetNode ,int depth);
	int random(int low, int high);
	/*bool isPositionPlotted(sf::Vector2f position)
	{
		for (int i = 0; i < plots.size(); i++)
		{
			if (plots[i] == position)
			{
				return true;
			}
		}

		return false;
	}*/


private:
	int* typeOfRoads;
	int roadLength = 20;
	int currentChunk;
	int rType = 1;
	sf::RenderWindow* refWindow;
	
};