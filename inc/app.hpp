#pragma once

/* glew must be loaded before anything else loads gl.h */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class App
{
private:
	GLFWwindow *win;
	int width, height;

public:
	App();
	void init(void);
	void run(void);
};
