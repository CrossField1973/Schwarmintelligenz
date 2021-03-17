#pragma once
#include "Vector.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

class Agent {
public:
    Vector location;
    Vector velocity;
    Vector acceleration;
    float maxSpeed;
    float maxForce;
    int w_width;
    int w_height;
    Agent() {}
    Agent(float x, float y, int world_width, int world_height);
    void applyForce(const Vector& force);
    Vector separation(const vector<Agent>& Agents);
    Vector align(const vector<Agent>& Agents);
    Vector cohesion(const vector<Agent>& Agents);
    Vector seek(const Vector& v);
    void run(const vector<Agent>& v);
    void update();
    void swarm(const vector<Agent>& v);
    void edges();
    float angle(const Vector& v);
};