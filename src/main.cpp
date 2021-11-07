#include <stdio.h>
#include <stdlib.h>
#include <random>

#include "app.hpp"
#include "shader.hpp"
#include "simulation.hpp"

int main(int argc, char *argv[])
{
	struct bodies testBodies;
	testBodies.x = {0,50};
	testBodies.y = {0,0};
	testBodies.z = {0,0};
	testBodies.mass = {6.6e10,5e6};
	testBodies.accelerations = { {0,0,0}, {0,0,0} };
	testBodies.velocities = { {0,0,0}, {0,0,1} };

	std::minstd_rand rng;
	std::random_device dev;
	std::uniform_real_distribution<float> pos_dist(-100, 100);
	std::uniform_real_distribution<float> mass_dist(5e3, 9e9);
	std::normal_distribution<float> vel_dist(0);
	rng.seed(dev());

	for (int i = 0; i < 10; ++i)
	{
		testBodies.x.push_back(pos_dist(rng));
		testBodies.y.push_back(pos_dist(rng));
		testBodies.z.push_back(pos_dist(rng));
		testBodies.mass.push_back(mass_dist(rng));
		testBodies.accelerations.push_back({
				vel_dist(rng),
				vel_dist(rng),
				vel_dist(rng),
			});
		testBodies.velocities.push_back({
				vel_dist(rng),
				vel_dist(rng),
				vel_dist(rng),
			});
	}

	Simulation sim(&testBodies, 0.05f, testBodies.x.size());

	App *a = App::get_instance();
	a->init(&sim);
	a->run();
}

