#pragma once
using namespace std;

#include <vector>;
#include <iostream>

class LSystem
{
public:
	 LSystem();
	~LSystem();


	void applyRules(int iterations);
	

private:
	std::vector<char> variables = { 'A','B' };
	std::vector<char> output = { 'A' };
	
};
