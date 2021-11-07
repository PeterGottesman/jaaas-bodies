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
	testBodies.mass = {9.6e10,5e6};
	testBodies.accelerations = { {0,0,0}, {0,0,0} };
	testBodies.velocities = { {0,0,0}, {0,0,1} };
	testBodies.unmoving = {false, false};
	testBodies.radius = {std::log10(7.6e10)*0.5,
		std::log10(5e6) * 0.5};

	std::minstd_rand rng;
	std::random_device dev;
	std::uniform_real_distribution<float> pos_dist(-100, 100);
	std::uniform_real_distribution<float> mass_dist(5e2, 9e8);
	std::normal_distribution<float> vel_dist(0, 2);
	rng.seed(dev());

	for (int i = 0; i < 3; ++i)
	{
		testBodies.x.push_back(pos_dist(rng));
		testBodies.y.push_back(pos_dist(rng) * 0.05);
		testBodies.z.push_back(pos_dist(rng));

		double mass = mass_dist(rng);
		testBodies.mass.push_back(mass_dist(rng));
		testBodies.radius.push_back(std::log10(mass)*0.5);
		testBodies.unmoving.push_back(false);
		testBodies.accelerations.push_back({
				vel_dist(rng)*0.2,
				vel_dist(rng)*0.2,
				vel_dist(rng)*0.2,
			});
		testBodies.velocities.push_back({
				vel_dist(rng),
				vel_dist(rng)*0.25,
				vel_dist(rng),
			});
	}

	for (int i = 0; i < 9000; ++i)
	{
		double x, y, z;

		x = pos_dist(rng) * 100;
		y = pos_dist(rng) * 100;
		z = pos_dist(rng) * 100;

		if (std::abs(x) < 350)
			continue;

		if (std::abs(y) < 350)
			continue;
		
		if (std::abs(z) < 350)
			continue;

		testBodies.x.push_back(x);
		testBodies.y.push_back(y);
		testBodies.z.push_back(z);

		double mass = mass_dist(rng);
		testBodies.mass.push_back(mass_dist(rng) * 100);
		testBodies.radius.push_back(std::log10(mass));
		testBodies.unmoving.push_back(true);
		testBodies.accelerations.push_back({
				0
			});
		testBodies.velocities.push_back({
				0
			});
	}

	Simulation sim(&testBodies, 0.05f, testBodies.x.size());

	App *a = App::get_instance();
	a->init(&sim);
	a->run();
}

