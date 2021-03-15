#include "simulation.h"
#include <random>

void Simulation::Initialize(int numAgents)
{
    //Initialize Agents
    for (int i = 0; i < numAgents; i++)
    {
        Agent agent;
        agent.x = rand() % this->worldWith + 1;
        agent.y = rand() % this->worldHeight + 1;
        agent.affiliation = 1;
        agents.push_back(agent);
    }
}