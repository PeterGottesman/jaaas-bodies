#include <vector>
#pragma once

struct bodies{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> mass;
    
    std::vector<struct accelVector> accelerations;
    std::vector<struct velocityVector> velocities;

	std::vector<double> radius;
	std::vector<bool> unmoving;
};

struct forceVector{
    double Fx;
    double Fy;
    double Fz;
};

struct accelVector{
    double aX;
    double aY;
    double aZ;
};

struct velocityVector{
    double Vx;
    double Vy;
    double Vz;
};

class Simulation{
    public:
        struct bodies * body;
        std::vector<struct forceVector> forces;
        double G;
        double delta;
        int num_points;

		Simulation(struct bodies * b, double changeInTime, int numPoints);

		void calculateForces();
		struct forceVector getForces(int i, int j);
        void calculateAccelerations(int i);
        void calculatePosition(int i);
        void calculateVelocities(int i);
        void nextIteration();
};
