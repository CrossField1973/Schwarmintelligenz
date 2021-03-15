#pragma once
#include <vector>
#include "agent.h"

class Simulation
{
public:
	std::vector<Agent> agents = {};
	int worldWith;
	int worldHeight;
};