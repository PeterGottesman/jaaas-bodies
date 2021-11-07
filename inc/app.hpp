#pragma once

/* glew must be loaded before anything else loads gl.h */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

class App
{
private:
	GLFWwindow *win;
	int width, height;

	glm::vec3 cam;
	glm::vec3 look_dir;

	static App *singleton;

public:
	App() {}
	void init(void);
	void run(void);
	void move_cam(glm::fvec3 dir);
	void rotate_cam(glm::fvec3 axis);

	static void keyhandler(GLFWwindow *win, int key, int scancode, int action, int mods);
	static App *get_instance(void);
};
