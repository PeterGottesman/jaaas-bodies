#pragma once

#include <vector>
#include "app.hpp"

class Planet
{
private:
	std::vector<glm::fvec3> verts;
	std::vector<glm::fvec3> colors;
	std::vector<glm::fvec3> norms;
	std::vector<glm::uvec3> tris;

	float radius;
	
	unsigned int vao, vbo[16];

	void subdivide(void);
	void init_mesh(void);

public:
	Planet(bool simple, float radius = 1.0f);
	void draw(void);
};
