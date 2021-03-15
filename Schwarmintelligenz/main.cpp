#include "agent.h"
#include <iostream>
#include <random>




int main()
{
	int width = 1000;
	int height = 1000;
	vector<Agent> agents;

	std::random_device random_device;
	std::mt19937 random_engine(random_device());
	std::uniform_real_distribution<double> distribution_0_1000(0 ,1000);

	
	for (int i = 0; i < 50; i++)
	{
	
		Agent agent = Agent(distribution_0_1000(random_engine), distribution_0_1000(random_engine), 1000, 1000);
		agents.push_back(agent);
	}

	for(;;)
	{
		for (int l = 0; l < agents.size(); l++)
		{
			agents[l].edges();
			agents[l].apply_behaivor(agents);
			agents[l].update();
			agents[l].show(l);
		}
	}

	
	return 0;
}
