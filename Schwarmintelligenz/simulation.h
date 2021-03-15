#pragma once
#include <vector>
#include "agent.h"
#include "logic.h"

class Simulation
{
public:
	Logic logic;
	std::vector<Agent> agents = {};
	int worldWith = 700;
	int worldHeight = 500;
	Simulation(int numAgents);
	void update();
};