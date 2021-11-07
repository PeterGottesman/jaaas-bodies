#include <stdio.h>
#include <stdlib.h>

#include "app.hpp"
#include "shader.hpp"
#include "simulation.hpp"

int main(int argc, char *argv[])
{
	struct bodies testBodies;
	testBodies.x = {-15,0};
	testBodies.y = {0,0};
	testBodies.z = {5,-3};
	testBodies.mass = {2e6,5e6};
	testBodies.accelerations = { {0,0,0}, {0,0,0} };
	testBodies.velocities = { {0,0,0}, {0,0,0} };

	Simulation sim(&testBodies, 0.05f, 2);

	App *a = App::get_instance();
	a->init(&sim);
	a->run();
}

