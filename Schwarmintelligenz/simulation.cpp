#include "simulation.h"
#include <random>

Simulation::Simulation(int numAgents, int width, int height) : fps(0), mspf(0), worldWidth(0), worldHeight(0)
{
	//Initialize Agents
	worldWidth = width;
	worldHeight = height;

	for (int i = 0; i < numAgents / 2; i++)
	{
		Agent agent(rand() % this->worldWidth + 1, rand() % this->worldHeight + 1, worldWidth, worldHeight, false);
		agents.push_back(agent);
	}

	for (int i = 0; i < numAgents/2; i++)
	{
		Agent  agent(rand() % this->worldWidth + 1, rand() % this->worldHeight + 1, worldWidth, worldHeight, true);
		agents.push_back(agent);
	}
	timer.reset();
}

void Simulation::update()
{
	// update timer
	timer.tick();
	calculateFrameStatistics();
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		agents[i].swarm(agents);
	}
	for (unsigned int l = 0; l < agents.size(); l++)
	{
		agents[l].update(timer.getDeltaTime());
		agents[l].edges();
	}
}

void Simulation::calculateFrameStatistics()
{
	static int nFrames;				    // number of frames seen
	static double elapsedTime;		    // time since last call
	nFrames++;

	// compute average statistics over one second
	if ((timer.getTotalTime() - elapsedTime) >= 1.0)
	{
		// set fps and mspf
		fps = nFrames;
		mspf = 1000.0 / (double)fps;

		// reset
		nFrames = 0;
		elapsedTime += 1.0;
	}
}