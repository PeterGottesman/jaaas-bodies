#include "simulation.hpp"
#include <stdlib.h>
#include <math.h>

void Simulation::calculateForces(){
	//initialize forces to be 0
	for(int i = 0; i < num_points; i++){
		for(int j = i+1; j < num_points; j++){
			struct forceVector f = getForces(i,j);

			forces[i].Fx += f.Fx;
			forces[j].Fx -= f.Fx;

			forces[i].Fy += f.Fy;
			forces[j].Fy -= f.Fy;

			forces[i].Fz += f.Fz;
			forces[j].Fz -= f.Fz;

		}
	}
}

struct forceVector Simulation::getForces(int i, int j){
	struct forceVector currentForces;

	double x1 = body->x[i];
	double y1 = body->y[i];
	double z1 = body->z[i];
	double mass1 = body->mass[i];

	double x2 = body->x[j];
	double y2 = body->y[j];
	double z2 = body->z[j];
	double mass2 = body->mass[j];

	/*Dont have to square root, because square root of distance
	  cancels out when we do "r^2" in Force equation*/
	double distanceX = (x2-x1);
	double distanceY = (y2-y1);
	double distanceZ = (z2-z1);

	double totalDistance = std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

	double forceX = (G*mass1*mass2*distanceX) / (totalDistance * totalDistance * totalDistance);
	double forceY = (G*mass1*mass2*distanceY) / (totalDistance * totalDistance * totalDistance);
	double forceZ = (G*mass1*mass2*distanceZ) / (totalDistance * totalDistance * totalDistance);

	if (totalDistance < 1)
	{
		forceX = forceY = forceZ = 0;
		forces[i] = forces[j] = {0};
		body->velocities[i] = body->velocities[j] = {0};
		body->accelerations[i] = body->accelerations[j] = {0};
	}

	currentForces.Fx = forceX;
	currentForces.Fy = forceY;
	currentForces.Fz = forceZ;

	return currentForces;
}

//Should only be called after calculateForces is called
void Simulation::calculateAccelerations(){

	//calculate x y z acceleration for all points
	for(int i = 0; i < num_points; i++){
		double mass = body->mass[i];

		double accelX = forces[i].Fx/mass;
		double accelY = forces[i].Fy/mass;
		double accelZ = forces[i].Fz/mass;

		body->accelerations[i].aX = accelX;
		body->accelerations[i].aY = accelY;
		body->accelerations[i].aZ = accelZ;
	}

}

//should be called last
void Simulation::calculatePosition(){

	//calculate new x y z position for every point
	for(int i = 0; i < num_points; i++){
		body->x[i] += body->velocities[i].Vx * delta;
		body->y[i] += body->velocities[i].Vy * delta;
		body->z[i] += body->velocities[i].Vz * delta;
	}
}

//should only be called after getAcceleration
void Simulation::calculateVelocities(){

	//calculate x y z velocities for all points
	for(int i = 0; i < num_points; i++){
		body->velocities[i].Vx += body->accelerations[i].aX * delta;
		body->velocities[i].Vy += body->accelerations[i].aY * delta;
		body->velocities[i].Vz += body->accelerations[i].aZ * delta;
	}
}

Simulation::Simulation(struct bodies * b, double changeInTime, int numPoints)
{
	body = b;
	G = 6.6e-9;
	delta = changeInTime;
	num_points = numPoints;

	forces.resize(num_points);
};

//only function that should be called by graphics
void Simulation::nextIteration(){
	calculateForces();
	calculateAccelerations();
	calculateVelocities();
	calculatePosition();
}
