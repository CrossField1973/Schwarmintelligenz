#pragma once

#include <vector>
#include "agent.h"
#include "logic.h"
#include "timer.h"

class Simulation
{
public:
	Logic logic;
	Timer timer;
	int fps;
	double mspf;
	std::vector<Agent> agents = {};
	int worldWith = 700;
	int worldHeight = 500;
	Simulation(int numAgents);
	void update();

	void calculateFrameStatistics();
};