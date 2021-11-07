#include <iostream>
#include "app.hpp"
#include "planet.hpp"
#include <glm/gtc/noise.hpp>

Planet::Planet(bool simple, float radius) : radius(radius)
{
	// Initial verts/tris shamelessly stolen from
	// https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
	const float X=.525731112119133606f * radius;
	const float Z=.850650808352039932f * radius;
	const float N=0.f;

	verts = {
		{-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
		{N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
		{Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};

	tris = {
		{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
		{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
		{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
		{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(16, vbo);

	if (!simple)
		for (int i = 0; i < 4; i++)
			subdivide();

	init_mesh();
}

void Planet::subdivide()
{
	std::vector<glm::uvec3> tmptris;


	for (auto t : tris)
	{
		// for each triangle, split into four tris
		verts.push_back(glm::normalize(verts[t.x] + verts[t.y]) * radius);
		verts.push_back(glm::normalize(verts[t.y] + verts[t.z]) * radius);
		verts.push_back(glm::normalize(verts[t.x] + verts[t.z]) * radius);
		unsigned int m2 = verts.size()-1;
		unsigned int m1 = m2-1;
		unsigned int m0 = m1-1;

		tmptris.push_back({t.x, m0, m2});
		tmptris.push_back({m0, t.y, m1});
		tmptris.push_back({m2, m1, t.z});
		tmptris.push_back({m0, m1, m2});
	}

	tris = tmptris;
}

void Planet::init_mesh()
{
	// Clear generated fields
	colors.clear();
	norms.clear();

	// Generate colors/norms
	for (auto&& v : verts)
	{
		glm::fvec3 noise = {
			glm::simplex(2.0f * v) * 0.25 +
			glm::simplex(v) * 0.15,

			glm::simplex(2.0f * v) * 0.09,

			glm::simplex(v) * 0.25 +
			glm::simplex(2.0f * v) * 0.25,
		};

		colors.push_back(glm::fvec3({0.7, 0.5, 0.35}) + noise * 0.5f);

		v += noise;
		norms.push_back(glm::normalize(v));
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * verts.size(), &verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tris[0]) * tris.size(), &tris[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0]) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(norms[0]) * norms.size(), &norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Planet::draw(void)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, tris.size() * tris[0].length(), GL_UNSIGNED_INT, 0);
}
