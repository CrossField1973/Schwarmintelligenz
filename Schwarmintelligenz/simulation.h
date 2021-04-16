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
	float speed;
	std::vector<Agent> agents = {};
	int worldWidth;
	int worldHeight;
	int numAgents;
	int numAgentsA;
	int numAgentsB;
	unsigned int selectedAgent = 0;
	Simulation(int numAgents, int width, int height);
	void update();

	void calculateFrameStatistics();
};