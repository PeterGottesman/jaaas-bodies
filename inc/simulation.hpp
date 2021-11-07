#include <vector>
#pragma once

struct bodies{
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<float> mass;
    
    std::vector<struct accelVector> accelerations;
    std::vector<struct velocityVector> velocities;
};

struct forceVector{
    float Fx;
    float Fy;
    float Fz;
};

struct accelVector{
    float aX;
    float aY;
    float aZ;
};

struct velocityVector{
    float Vx;
    float Vy;
    float Vz;
};