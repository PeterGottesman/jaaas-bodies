#include <stdio.h>
#include <stdlib.h>

#include "app.hpp"
#include "shader.hpp"
#include "simulation.hpp"

int main(int argc, char *argv[])
{
	struct bodies testBodies;
	testBodies.x = {0,5};
	testBodies.y = {0,5};
	testBodies.z = {0,0};
	testBodies.mass = {50,10};
	testBodies.accelerations{ {0,0,0}, {0,0,0} };
	testBodies.velocities{ {0,0,0}, {2,3,0} };
;
	App *a = App::get_instance();
	a->run();
}

