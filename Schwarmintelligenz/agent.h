#pragma once
#include "Vector.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

class Agent {
public:
    bool swarmB;
    Vector location;
    Vector velocity;
    Vector acceleration;
    float overtakenPercentage;
    float maxSpeed;
    float maxForce;
    int w_width;
    int w_height;
    int neighbor;
    Agent() {}
    Agent(float x, float y, int world_width, int world_height);
    Agent(float x, float y, int world_width, int world_height, bool checkSwarmB);
    void applyForce(const Vector& force);
    void getNeighbor(const vector<Agent>& Agents);
    Vector separation(const vector<Agent>& Agents, double dt);
    Vector align(const vector<Agent>& Agents);
    Vector cohesion(const vector<Agent>& Agents);
    Vector seek(const Vector& v);
    void update(double dt);
    void swarm(const vector<Agent>& v, double dt);
    void edges();
    float angle(const Vector& v);
};
