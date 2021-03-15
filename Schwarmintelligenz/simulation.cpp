#include "simulation.h"
#include <random>

Simulation::Simulation(int numAgents)
{
    //Initialize Agents
    for (int i = 0; i < numAgents; i++)
    {
        Agent agent(rand() % this->worldWith + 1, rand() % this->worldHeight + 1, 500, 700);
        agent.setAffiliation(1);
        agents.push_back(agent);
    }
}

void Simulation::update()
{

}