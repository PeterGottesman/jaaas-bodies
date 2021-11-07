#include "simulation.hpp"
#include <stdlib.h>

void Simulation::calculateForces(){

	//initialize forces to be 0
	std::vector<struct forceVector> forces(num_points,{0,0,0});

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

	float x1 = body->x[i];
	float y1 = body->y[i];
	float z1 = body->z[i];
	float mass1 = body->mass[i];

	float x2 = body->x[j];
	float y2 = body->y[j];
	float z2 = body->z[j];
	float mass2 = body->mass[j];

	/*Dont have to square root, because square root of distance
	  cancels out when we do "r^2" in Force equation*/
	float distanceX = (x2-x1);
	float distanceY = (y2-y1);
	float distanceZ = (z2-z1);

	float totalDistance = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1);

	float forceX = (G*mass1*mass2*distanceX) / totalDistance;
	float forceY = (G*mass1*mass2*distanceY) / totalDistance;
	float forceZ = (G*mass1*mass2*distanceZ) / totalDistance;

	currentForces.Fx = forceX;
	currentForces.Fy = forceY;
	currentForces.Fz = forceZ;

	return currentForces;
}

//Should only be called after calculateForces is called
void Simulation::calculateAccelerations(){

	//calculate x y z acceleration for all points
	for(int i = 0; i < num_points; i++){
		float mass = body->mass[i];

		float accelX = forces[i].Fx/mass;
		float accelY = forces[i].Fy/mass;
		float accelZ = forces[i].Fz/mass;

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

Simulation::Simulation(struct bodies * b, float changeInTime, int numPoints)
{
	body = b;
	G = .01;
	delta = changeInTime;
	num_points = numPoints;
};

//only function that should be called by graphics
void Simulation::nextIteration(){
	calculateForces();
	calculateAccelerations();
	calculateVelocities();
	calculatePosition();
}
