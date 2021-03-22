#include "Agent.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;


	Agent::Agent(double x, double y, int height, int width)
	{
		posX = x;
		posY = y;

		random_device random_device;
		mt19937 random_engine(random_device());
		uniform_real_distribution<double> distribution_0_1(0, 1);


		velX = distribution_0_1(random_engine)*10;
		velY = distribution_0_1(random_engine)*10;
		
		accX = distribution_0_1(random_engine);
		accY = distribution_0_1(random_engine);

		
		Agent::width = width;
		Agent::height = height;

	}

	void Agent::update()
	{
		posX += velX;
		velX += accX;

		posY += velY;
		velY += accY;
	}

	void Agent::apply_behaivor(vector<Agent> agents)
	{
		vector <double> alignment = Agent::align(agents);
		vector <double> cohesion = Agent::cohesion(agents);
		vector <double> seperation = Agent::seperation(agents);
		double accX = Agent::getAccX();
		double accY = Agent::getAccY();

		double alX = alignment[0];
		double alY = alignment[1];

		double coX = (cohesion[0]);
		double coY = (cohesion[1]);

		double sepX = (seperation[0]);
		double sepY = (seperation[1]);

		cout << "al " << alX << endl;
		cout << "co " << coX << endl;
		cout << "sep " << sepX << endl;
		cout << "al " << alY << endl;
		cout << "co " << coY << endl;
		cout << "sep " << sepY << endl;

		accX = accX + alX + coX + +sepX;
		accX = accY + alY + coY + +sepY;


	
		Agent::setAccX((accX));
		Agent::setAccY((accY));
	}

	void Agent::edges()
	{
		if (Agent::getPosX() > Agent::width)
		{
			Agent::setPosX(0.0);
		}
		else if (Agent::getPosX() < 0)
		{
			Agent::setPosX((double)Agent::width);
		}

		if (Agent::getPosY() > Agent::height)
		{
			Agent::setPosY(0.0);
		}
		else if (Agent::getPosY() < 0)
		{
			Agent::setPosY((double)Agent::height);
		}

	}
	void Agent::show(int i)
	{
		cout << "POSX : " << Agent::getPosX() << " Agent : " << i << endl;
		cout << "POSY : " << Agent::getPosY() << " Agent : " << i << endl;
	}




