#include <math.h>
#include "Vector.h"

#define PI 3.141592635


void Vector::set(float i, float o)
{
    x = i;
    y = o;
}

void Vector::addVector(const Vector& v)
{
    x += v.x;
    y += v.y;
}

void Vector::addScalar(float s)
{
    x += s;
    y += s;
}

void Vector::subVector(const Vector& v)
{
    x -= v.x;
    y -= v.y;
}

Vector Vector::subTwoVector(const Vector& v, const Vector& v2)
{
    Vector
 tmp(v.x - v2.x, v.y - v2.y);
    return std::move(tmp);
}

void Vector::subScalar(float s)
{
    x -= s;
    y -= s;
}

void Vector::mulVector(const Vector& v)
{
    x *= v.x;
    y *= v.y;
}

void Vector::mulScalar(float s)
{
    x *= s;
    y *= s;
}

void Vector::divVector(const Vector& v)
{
    x /= v.x;
    y /= v.y;
}

void Vector::divScalar(float s)
{
    x /= s;
    y /= s;
}

void Vector::limit(double max)
{
    double size = magnitude();

    if (size > max) {
        set(x / size, y / size);
    }
}

float Vector::distance(const Vector& v)
{
    float dx = x - v.x;
    float dy = y - v.y;
    float dist = sqrt(dx*dx + dy*dy);
    return dist;
}

float Vector::dotProduct(const Vector& v)
{
    float dot = x * v.x + y * v.y;
    return dot;
}

float Vector::magnitude()
{
    return sqrt(x*x + y*y);
}

void Vector::setMagnitude(float x)
{
    normalize();
    mulScalar(x);
}

float Vector::angleBetween(const Vector& v)
{
    if (x == 0 && y == 0) return 0.0f;
    if (v.x == 0 && v.y == 0) return 0.0f;

    double dot = x * v.x + y * v.y;
    double v1mag = sqrt(x * x + y * y);
    double v2mag = sqrt(v.x * v.x + v.y * v.y);
    double amt = dot / (v1mag * v2mag);
    if (amt <= -1) {
        return PI;
    } else if (amt >= 1) {
        return 0;
    }
    float tmp = acos(amt);
    return tmp;
}

void Vector::normalize()
{
    float m = magnitude();

    if (m > 0) {
        set(x / m, y / m);
    } else {
        set(x, y);
    }
}

Vector Vector::copy(const Vector& v)
{
    Vector
 copy(v.x, v.y);
    return copy;
}