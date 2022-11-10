#include "LSystem.h"

LSystem::LSystem()
{
	variables = { 'A','B' };
	output = { 'A' };
}

LSystem::~LSystem()
{
}

void LSystem::applyRules(int iterations)
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
			case 'A':
			{
				tempOutput.push_back('A');
				tempOutput.push_back('B');
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
