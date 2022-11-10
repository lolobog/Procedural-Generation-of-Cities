#include "RoadManager.h"

RoadManager::RoadManager(int chunkID)
{
	variables = { 'C' ,'F','R','L','B'};
	output = { 'C' };
	RoadNetwork->setRoot(findChunkCenter(chunkID));
}

RoadManager::~RoadManager()
{
}

void RoadManager::applyRules(int iterations)
{
	for (auto c : output)
	{
		cout << c;
	}
	cout << '\n';

	while (iterations > 0)
	{
		vector<char> tempOutput;

		for (int i = 0; i < output.size(); i++)
		{
			switch (output[i])
			{
			
			case 'C':
			{
				tempOutput.push_back('F');
				tempOutput.push_back('R');
				tempOutput.push_back('L');
				tempOutput.push_back('B');

				break;
			}
			case 'F':
			{
				tempOutput.push_back('A');
				break;
			}
			case 'R':
			{
				tempOutput.push_back('A');
				break;
			}
			case 'L':
			{
				tempOutput.push_back('A');
				break;
			}
			case 'B':
			{
				tempOutput.push_back('A');
				break;
			}



			default:
				break;
			}
		}
		output = tempOutput;
		for (auto c : output)
		{
			cout << c;
		}
		cout << '\n';
		iterations--;
	}
}

NodeTree::NodeTree()
{
}

NodeTree::~NodeTree()
{
}
