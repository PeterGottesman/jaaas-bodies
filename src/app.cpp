#include <random>

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

App *App::singleton = nullptr;

App *App::get_instance()
{
	if (singleton == nullptr)
	{
		singleton = new App();
	}

	return singleton;
}

void App::keyhandler(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	App *a = App::get_instance();
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, GLFW_TRUE);
			break;
		case GLFW_KEY_Q:
			a->rotate_cam({0, 1, 0});
			break;
		case GLFW_KEY_E:
			a->rotate_cam({0, -1, 0});
			break;
		case GLFW_KEY_W:
			a->move_cam({0, 0, 1});
			break;
		case GLFW_KEY_A:
			a->move_cam({-1, 0, 0});
			break;
		case GLFW_KEY_S:
			a->move_cam({0, 0, -1});
			break;
		case GLFW_KEY_D:
			a->move_cam({1, 0, 0});
			break;
		case GLFW_KEY_LEFT_CONTROL:
			a->move_cam({0, -1, 0});
			break;
		case GLFW_KEY_SPACE:
			a->move_cam({0, 1, 0});
			break;
		default:
			break;
	}
}

void App::move_cam(glm::fvec3 dir)
{
	if (dir.z != 0)
	{
		cam += look_dir * dir.z;
	}
	else if (dir.x != 0)
	{
		cam += glm::cross(look_dir, {0.0f, 1.0f, 0.0f}) * dir.x;
	}
	else if (dir.y != 0)
	{
		cam += dir;
	}
}

void App::rotate_cam(glm::fvec3 axis)
{
	look_dir = glm::rotate(look_dir, glm::radians(5.0f), axis);
}

void App::run()
{
	int res_loc;

	// std::vector<glm::fvec3> positions;
	std::vector<Planet> planets;

	for (int i = 0; i < sim->num_points; ++i)
	{
		planets.push_back(std::log10(std::log(sim->body->mass[i])));
	}

	struct program *prog = loadShaders();

	glm::mat4 proj = glm::perspective(glm::radians(45.0), 4.0/3.0, 0.1, 10000.0);
	glm::mat4 model;
	glm::mat4 view;

	res_loc = glGetUniformLocation(prog->program_id, "proj");
	glUniformMatrix4fv(res_loc, 1, GL_FALSE, &proj[0][0]);

	glEnable(GL_DEPTH_TEST);

	float t = 0.0;
	while (!glfwWindowShouldClose(win))
	{
		sim->nextIteration();
		glClearColor(0.05, 0, 0.2, 0.1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::lookAt(
			cam,
			cam + look_dir,
			glm::vec3(0,1,0)  // up
			);


		res_loc = glGetUniformLocation(prog->program_id, "cam_dir");
		glUniform3f(res_loc, look_dir.x, look_dir.y, look_dir.z);

		res_loc = glGetUniformLocation(prog->program_id, "view");
		glUniformMatrix4fv(res_loc, 1, GL_FALSE, &view[0][0]);

		for (long i = 0; i < planets.size(); ++i)
		{
			glm::fvec3 pos = {
				sim->body->x[i],
				sim->body->y[i],
				sim->body->z[i],
			};
			model = glm::translate(glm::mat4(1.0f), pos);
			res_loc = glGetUniformLocation(prog->program_id, "model");
			glUniformMatrix4fv(res_loc, 1, GL_FALSE, &model[0][0]);
			planets[i].draw();
		}

		glfwPollEvents();
		glfwSwapBuffers(win);

		t += 0.01;
	}

	glfwDestroyWindow(win);
}

void App::init(Simulation *sim)
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

	cam = {5, 25, 25};
	look_dir = glm::normalize(cam * -1.0f);

	this->sim = sim;
}
