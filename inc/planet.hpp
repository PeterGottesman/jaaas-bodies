#pragma once

struct vertex
{
	float x, y, z;
};

struct triangle
{
	int v1, v2, v3;
};

class Planet
{
private:
	struct vertex *verts;
	struct triangle *tris;
	float radius;
	
	unsigned int vao, vbo[3];

public:
	Planet(float radius = 1.0f);
	void draw(void);
	struct vertex *get_verts() { return verts; }
	struct triangle *get_tris() { return tris; }
};
