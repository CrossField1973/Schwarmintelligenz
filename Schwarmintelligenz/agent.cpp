#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "agent.h"

#define PI 3.141592635


Agent::Agent(float x, float y, int world_width, int world_height)
{
    acceleration = Vector(0, 0);
    velocity = Vector(rand()%3 - 2, rand()%3 - 2);
    location = Vector(x, y);
    maxSpeed = 200;
    maxForce = 0.5;
    w_width = world_width;
    w_height = world_height;
}


void Agent::applyForce(const Vector& force)
{
    acceleration.addVector(force);
}

Vector Agent::separation(const vector<Agent>& Agents)
{
    float desiredseparation = 10;
    Vector steering(0, 0);
    int count = 0;
    for (int i = 0; i < Agents.size(); i++) {
        float d = location.distance(Agents[i].location);
        if ((d > 0) && (d < desiredseparation)) {
            Vector diff(0,0);
            diff = diff.subTwoVector(location, Agents[i].location);
            diff.normalize();
            diff.divScalar(d);      
            steering.addVector(diff);
            count++;
        }
    }
    if (count > 0)
        steering.divScalar((float)count);
    if (steering.magnitude() > 0) {
        steering.normalize();
        steering.mulScalar(maxSpeed);
        steering.subVector(velocity);
        steering.limit(maxForce);
    }
    return steering;
}

Vector Agent::align(const vector<Agent>& Agents)
{
    float neighbordist = 50; // Field of vision

    Vector sum(0, 0);
    int count = 0;
    for (int i = 0; i < Agents.size(); i++) {
        float d = location.distance(Agents[i].location);
        if ((d > 0) && (d < neighbordist)) { // 0 < d < 50
            sum.addVector(Agents[i].velocity);
            count++;
        }
    }
    if (count > 0) {
        sum.divScalar((float)count);
        sum.normalize();
        sum.mulScalar(maxSpeed);
        Vector steering;
        steering = steering.subTwoVector(sum, velocity);
        steering.limit(maxForce);
        return steering;
    } else {
        Vector temp(0, 0);
        return temp;
    }
}

Vector Agent::cohesion(const vector<Agent>& Agents)
{
    float neighbordist = 50;
    Vector sum(0, 0);
    int count = 0;
    for (int i = 0; i < Agents.size(); i++) {
        float d = location.distance(Agents[i].location);
        if ((d > 0) && (d < neighbordist)) {
            sum.addVector(Agents[i].location);
            count++;
        }
    }
    if (count > 0) {
        sum.divScalar(count);
        return seek(sum);
    } else {
        Vector temp(0,0);
        return temp;
    }
}

Vector Agent::seek(const Vector& v)
{
    Vector desired;
    desired.subVector(v);
    desired.normalize();
    desired.mulScalar(maxSpeed);
    acceleration.subTwoVector(desired, velocity);
    acceleration.limit(maxForce);
    return acceleration;
}

void Agent::update(double dt)
{
    acceleration.mulScalar(0.4);
    velocity.addVector(acceleration);
    velocity.limit(maxSpeed * dt);
    velocity.mulScalar(100 * dt);
    location.addVector(velocity);
    acceleration.mulScalar(0);
}

void Agent::run(const vector <Agent>& v, double dt)
{
    swarm(v);
    update(dt);
    edges();
}

void Agent::swarm(const vector<Agent>& v)
{
    Vector sep = separation(v);
    Vector ali = align(v);
    Vector coh = cohesion(v);
    sep.mulScalar(1.5);
    ali.mulScalar(1.0);
    coh.mulScalar(1.0);
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
}

void Agent::edges()
{
    if (location.x < 0)    location.x += w_width;
    if (location.y < 0)    location.y += w_height;
    if (location.x > w_width) location.x -= w_width;
    if (location.y > w_height) location.y -= w_height;
}

float Agent::angle(const Vector& v)
{
    float angle = (float)(atan2(v.y, v.x) * 180 / PI);
    return angle;
}