#pragma once
#include <vector>
#include "agent.h"
#include "logic.h"

class Simulation
{
public:
	Logic logic;
	std::vector<Agent> agents = {};
	int worldWith;
	int worldHeight;
	void Initialize(int numAgents);
	void update();
};