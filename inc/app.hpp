#pragma once

/* glew must be loaded before anything else loads gl.h */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

#include "simulation.hpp"

class App
{
private:
	GLFWwindow *win;
	Simulation *sim;

	int width, height;

	glm::vec3 cam;
	glm::vec3 look_dir;

	static App *singleton;

	enum direction {
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_FRONT,
		DIR_BACK,
	};

public:
	App() {}
	void init(Simulation *sim);
	void run(void);
	void move_cam(direction dir);
	void rotate_cam(direction axis);

	static void keyhandler(GLFWwindow *win, int key, int scancode, int action, int mods);
	static App *get_instance(void);
};
