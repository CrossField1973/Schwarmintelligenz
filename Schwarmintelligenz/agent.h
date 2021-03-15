#pragma once
#include <vector> 
#include <math.h>

using namespace std;
class Agent
{
public:
	Agent(double x, double y, int height, int width);


	~Agent() {};


	void update();

	void show(int i);

	void apply_behaivor(std::vector<Agent> agents);

	void edges();

	/**********************************/
	/*		getter and setter		 */
    /********************************/

	double getPosX()
	{
		return posX;
	}

	double getPosY()
	{
		return posY;
	}
	void setPosX(double p)
	{
		posX = p;
	}
	void setPosY(double p)
	{
		posY = p;
	}

	double getAccX()
	{
		return accX;
	}

	double getAccY()
	{
		return accX;
	}
	void setAccX(double a)
	{
		accX = a;
	}
	void setAccY(double a)
	{
		posY = a;
	}

	void setAffiliation(int a)
	{
		affiliation = a;
	}

	int getWidth()
	{
		return width;
	}

	int getPerception()
	{
		return perception; 
	}

	double getVelX()
	{
		return velX;
	}

	double getVelY()
	{
		return velY;
	}

	int getMaxSpeed()
	{
		return max_speed;
	}

	double getMaxForce()
	{
		return max_force;
	}

	int getAffiliation()
	{
		return affiliation;
	}




private:
	/**********************************/
	/*			fields				 */
	/********************************/
	double posX;
	double posY;
	double velX;
	double velY;
	double accX;
	double accY;
	vector<double> velocity;
	vector<double> acceleration;
	double max_force = 0.3;
	int max_speed = 5;
	int perception = 100;
	int width;
	int height;
	int affiliation;

	/**********************************/
	/*		 private methods		 */
	/********************************/

	vector<double> align(vector<Agent> agents)
	{
		vector<double> steering{ 0.0, 0.0 };
		int total = 0;
		double avgX = 0.0;
		double avgY = 0.0;

		for (int i = 0; i < agents.size(); i++)
		{
			double posX = agents[i].getPosX() - this->getPosX();
			double posY = agents[i].getPosY() - this->getPosY();

			if (sqrt(posX * posX + posY * posY) < this->getPerception())
			{
				avgX += agents[i].getVelX();
				avgY += agents[i].getVelY();
				total ++;
			}
		}
		if (total > 0)
		{
			avgX = avgX / total;
			avgY = avgY / total;
			if (sqrt(avgX * avgX + avgY * avgY) > 0)
			{
				avgX = (avgX / (sqrt(avgX * avgX + avgY * avgY))) * this->getMaxSpeed();
				avgY = (avgY / (sqrt(avgX * avgX + avgY * avgY))) * this->getMaxSpeed();
			}
			steering[0] = avgX - this->getVelX();
			steering[1] = avgY - this->getVelY();
		}
		

		return steering;
	}

	vector<double> cohesion(vector<Agent> agents)
	{
		vector<double> steering{ 0.0, 0.0 };
		int total = 0;
		double cmX = 0.0;
		double cmY = 0.0;
		double comX;
		double comY;
		for (int i = 0; i < agents.size(); i++)
		{
			double posX = agents[i].getPosX() - this->getPosX();
			double posY = agents[i].getPosY() - this->getPosY();

			if (sqrt(posX * posX + posY * posY) < this->getPerception())
			{
				cmX += agents[i].getVelX();
				cmY += agents[i].getVelY();
				total ++;
			}
		}
		if (total > 0)
		{
			cmX = cmX / total;
			cmY = cmY / total;
			comX = cmX / this->getPosX();
			comY = cmX / this->getPosX();

			if (sqrt(comX * comX + comY * comY) > 0)
			{
				comX = (comX / (sqrt(comX * comX + comY * comY))) * this->getMaxSpeed();
				comY = (comY / (sqrt(comX * comX + comY * comY))) * this->getMaxSpeed();
			}
			steering[0] = comX - this->getVelX();
			steering[1] = comY - this->getVelY();
			if (sqrt(steering[0] * steering[0] + steering[1] * steering[1]) > this->getMaxForce() && sqrt(steering[0] * steering[0] + steering[1] * steering[1]) > 0)
			{
				steering[0] = (steering[0] / (sqrt(steering[0] * steering[0] + steering[1] * steering[1]))) * this->getMaxForce();
				steering[1] = (steering[1] / (sqrt(steering[0] * steering[0] + steering[1] * steering[1]))) * this->getMaxForce();
			}
		}

		return steering;
	}

	vector<double> seperation(vector<Agent> agents)
	{
		vector<double> steering{ 0.0, 0.0 };
		int total = 0;
		double avgX = 0.0;
		double avgY = 0.0;
		double distance;
		double diffX;
		double diffY;
		for (int i = 0; i < agents.size(); i++)
		{
			double posX = agents[i].getPosX() - this->getPosX();
			double posY = agents[i].getPosY() - this->getPosY();
			distance = sqrt(posX * posX + posY * posY);
			if (agents[i].getPosX() != this->getPosX() && agents[i].getPosY() != this->getPosY() && distance < this->getPerception())
			{
				diffX = this->getPosX() - agents[i].getPosX();
				diffY = this->getPosY() - agents[i].getPosY();
				diffX = diffX / distance;
				diffY = diffY / distance;
				avgX += diffX;
				avgY += diffY;
				total++;
			}
		}
		if (total > 0)
		{
			avgX = avgX / total;
			avgY = avgY / total;
			if (sqrt(steering[0] * steering[0] + steering[1] * steering[1]) > 0)
			{
				avgX = (avgX / (sqrt(avgX * avgX + avgY * avgY))) * this->getMaxSpeed();
				avgY = (avgY / (sqrt(avgX * avgX + avgY * avgY))) * this->getMaxSpeed();
			}
			steering[0] = avgX - this->getVelX();
			steering[1] = avgY - this->getVelY();
			if (sqrt(steering[0] * steering[0] + steering[1] * steering[1]) > this->getMaxForce() && sqrt(steering[0] * steering[0] + steering[1] * steering[1]) > 0)
			{
				steering[0] = (steering[0] / (sqrt(steering[0] * steering[0] + steering[1] * steering[1]))) * this->getMaxForce();
				steering[1] = (steering[1] / (sqrt(steering[0] * steering[0] + steering[1] * steering[1]))) * this->getMaxForce();
			}
		}
		return steering;
	}


	

};