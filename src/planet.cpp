#include <iostream>
#include "app.hpp"
#include "planet.hpp"

Planet::Planet(float radius) : radius(radius)
{
	verts = new struct vertex[8] {
		{-0.5, 0.5, -0.5},		// 0 top back left
		{-0.5, 0.5, 0.5},		// 1 top front left
		{0.5, 0.5, -0.5},		// 2 top back right
		{0.5, 0.5, 0.5},		// 3 top front right
		{-0.5, -0.5, -0.5},		// 4 bottom back left
		{-0.5, -0.5, 0.5},		// 5 bottom front left
		{0.5, -0.5, -0.5},		// 6 bottom back right
		{0.5, -0.5, 0.5},		// 7 bottom front right
	};

	struct vertex *colors = new struct vertex[8] {
		{0, 0, 1},
		{1, 0, 1},
		{0, 1, 1},
		{1, 0, 0},

		{0, 1, 0},
		{1, 1, 1},
		{1, 1, 0},
		{1, 1, 1},
	};

	tris = new struct triangle[12] {
		// top
		{0, 1, 2},
		{2, 1, 3},

		// bottom
		{4, 5, 6},
		{7, 6, 5},

		// left 
		{0, 4, 1},
		{1, 4, 5},

		// right 
		{3, 7, 2},
		{2, 7, 6},

		// front
		{1, 5, 3},
		{3, 5, 7},

		// back
		{0, 4, 2},
		{2, 4, 6},
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glGenBuffers(3, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, tris, GL_STATIC_DRAW);
}

void Planet::draw(void)
{
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
