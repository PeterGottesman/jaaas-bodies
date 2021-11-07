#include <stdio.h>
#include <stdlib.h>

#include "app.hpp"
#include "shader.hpp"
#include "simulation.hpp"

int main(int argc, char *argv[])
{
	struct *bodies testBodies;
	bodies->x = {0,5}
	bodies->y = {0,5}
	bodies->z = {0,0}
	bodies->mass = {50,10}
	bodies->accelerations{ {0,0,0}, {0,0,0} }
	bodies->velocities{ {0,0,0}, {2,3,0} }

	App *a = App::get_instance();
	a->run();
}

