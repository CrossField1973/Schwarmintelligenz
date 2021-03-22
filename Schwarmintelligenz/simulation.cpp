#include "simulation.h"
#include <random>

Simulation::Simulation(int numAgents) : fps(0), mspf(0)
{
    //Initialize Agents


    for (int i = 0; i < numAgents; i++)
    {
        Agent agent(rand() % this->worldWith + 1, rand() % this->worldHeight + 1, 800, 600);
        //agent.setAffiliation(1);
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
		agents[i].run(agents, timer.getDeltaTime());
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