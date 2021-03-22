#pragma once

#include <vector>
#include "agent.h"
#include "timer.h"

class Simulation
{
public:
	Timer timer;
	int fps;
	double mspf;
	std::vector<Agent> agents = {};
	int worldWith = 1200;
	int worldHeight = 720;
	unsigned int selectedAgent = 0;
	Simulation(int numAgents);
	void update();

	void calculateFrameStatistics();
};