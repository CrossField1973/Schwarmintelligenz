#include "simulation.h"
#include <random>
#include <string>

Simulation::Simulation(int numAgents, int width, int height, __int64 seed)
	: fps(0), mspf(0), worldWidth(width), worldHeight(height), speed(1), numAgents(0), numAgentsA(numAgents / 2), numAgentsB(numAgents / 2), m_seed(seed)
{
	this->numAgents = numAgents;

	if (m_seed == -1) {
		m_seed = time(NULL);
		srand(m_seed);
	} 
	else
	{
		srand(m_seed);
	}

	//Initialize Agents
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
	int tmpNumAgentA = 0;
	int tmpNumAgentB = 0;

	timer.tick();
	calculateFrameStatistics();
	for (unsigned int i = 0; i < agents.size(); i++)
	{
		agents[i].swarm(agents, timer.getDeltaTime() * speed);

		// count agents in swarm
		if (agents[i].swarmB) 
		{
			tmpNumAgentB++;
		}
		else
		{
			tmpNumAgentA++;
		}
	}
	for (unsigned int l = 0; l < agents.size(); l++)
	{
		agents[l].update(timer.getDeltaTime() * speed);
		agents[l].edges();
	}

	numAgentsA = tmpNumAgentA;
	numAgentsB = tmpNumAgentB;
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
