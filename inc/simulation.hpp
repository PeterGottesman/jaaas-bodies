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

class Simulation{
    public:
        struct bodies * body;
        std::vector<struct forceVector> forces;
        float G;
        float delta;
        int num_points;

		Simulation(struct bodies * b, float changeInTime, int numPoints);

		void calculateForces();
		struct forceVector getForces(int i, int j);
        void calculateAccelerations();
        void calculatePosition();
        void calculateVelocities();
        void nextIteration();
};
