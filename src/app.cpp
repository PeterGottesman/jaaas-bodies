#include "app.hpp"
#include "shader.hpp"
#include "planet.hpp"

static struct program *loadShaders()
{
	struct shader *shaders[2];
	struct program *prog;
	shaders[0] = shader_load("resources/fs.frag", SHADER_FRAG);
	shaders[1] = shader_load("resources/vs.vert", SHADER_VERT);

	prog = program_load(shaders, 2);
	return prog;
}

static void keyhandler(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, GLFW_TRUE);
			break;
		case GLFW_KEY_R:
			// reload shaders
			break;
		default:
			break;
	}
}

App::App() {}

void App::run()
{
	int res_loc;

	Planet p(0.5);

	struct program *prog = loadShaders();

	res_loc = glGetUniformLocation(prog->program_id, "uResolution");
	glUniform2f(res_loc, width, height);

	glm::mat4 proj = glm::perspective(glm::radians(45.0), 4.0/3.0, 0.1, 10.0);

	glm::mat4 view = glm::lookAt(
		glm::vec3(4,0,3), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 model = glm::mat4(1.0f);

	res_loc = glGetUniformLocation(prog->program_id, "model");
	glUniformMatrix4fv(res_loc, 1, GL_FALSE, &model[0][0]);
	res_loc = glGetUniformLocation(prog->program_id, "proj");
	glUniformMatrix4fv(res_loc, 1, GL_FALSE, &proj[0][0]);
	res_loc = glGetUniformLocation(prog->program_id, "view");
	glUniformMatrix4fv(res_loc, 1, GL_FALSE, &view[0][0]);

	glEnable(GL_DEPTH_TEST);

	float t = 0.0;
	while (!glfwWindowShouldClose(win))
	{
		glClearColor(0.05, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = glm::lookAt(
			glm::vec3(4,3,3), // camera
			glm::vec3(0,0,0), // lookat
			glm::vec3(0,1,0)  // up
			);

		res_loc = glGetUniformLocation(prog->program_id, "view");
		glUniformMatrix4fv(res_loc, 1, GL_FALSE, &view[0][0]);

		// glDrawArrays(GL_TRIANGLES, 0, 3);
		p.draw();
		// glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(win);

		t += 0.01;
	}

	glfwDestroyWindow(win);
}

void App::init()
{
	glfwInit();

	width = height = 400;

	win = glfwCreateWindow(width, height, "Window Title", NULL, NULL);
	glfwSetKeyCallback(win, keyhandler);

	if (!win)
	{
		fprintf(stderr, "Failed to create window\n");
		return;
	}

	glfwGetWindowSize(win, &width, &height);

	/* Make opengl context the active context for this thread */
	glfwMakeContextCurrent(win);
	glewInit();

	glViewport(0, 0, width, height);

}
