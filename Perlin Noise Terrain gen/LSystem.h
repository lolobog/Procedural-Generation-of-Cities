#pragma once
using namespace std;

#include <vector>;
#include <iostream>

class LSystem
{
public:
	 LSystem();
	~LSystem();


	virtual void applyRules(int iterations);
	
protected:
	std::vector<char> variables;
	std::vector<char> output;

private:
	
	
};
