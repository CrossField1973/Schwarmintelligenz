#pragma once
#include <iostream>
using namespace std;

class Vector {
public:
    float x;
    float y;

    //Constructors
    Vector() {}

    Vector(float xComp, float yComp)
    {
        x = xComp;
        y = yComp;
    }

    //Mutator Functions
    void set(float x, float y);

    //Scalar functions scale a vector by a float
    void addVector(const Vector& v);
    void addScalar(float x);

    void subVector(const Vector& v);
    Vector subTwoVector(const Vector& v, const Vector& v2);
    void subScalar(float x);

    void mulVector(const Vector& v);
    void mulScalar(float x);

    void divVector(const Vector& v);
    void divScalar(float x);

    void limit(double max);

    //Calculating Functions
    float distance(const Vector& v);
    float dotProduct(const Vector& v);
    float magnitude();
    void setMagnitude(float x);
    float angleBetween(const Vector& v);
    void normalize();

    Vector copy(const Vector& v);
};