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
		singleton->init();
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
		cam += glm::cross({0.0f, 1.0f, 0.0f}, look_dir) * dir.x;
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

	std::vector<glm::fvec3> positions;
	std::vector<Planet> planets;

	for (int i = 0; i < 5; ++i)
	{
		std::random_device rng;
		std::uniform_real_distribution<float> rad_dist(0, 2);
		planets.push_back({rad_dist(rng)});

		std::uniform_real_distribution<float> pos_dist(-5, 5);
		positions.push_back({
				pos_dist(rng),
				pos_dist(rng),
				pos_dist(rng),
			});
	}

	struct program *prog = loadShaders();

	res_loc = glGetUniformLocation(prog->program_id, "uResolution");
	glUniform2f(res_loc, width, height);

	glm::mat4 proj = glm::perspective(glm::radians(45.0), 4.0/3.0, 0.1, 100.0);
	glm::mat4 model;
	glm::mat4 view;

	res_loc = glGetUniformLocation(prog->program_id, "proj");
	glUniformMatrix4fv(res_loc, 1, GL_FALSE, &proj[0][0]);

	glEnable(GL_DEPTH_TEST);

	float t = 0.0;
	while (!glfwWindowShouldClose(win))
	{
		glClearColor(0.05, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = glm::lookAt(
			cam,
			cam + look_dir,
			glm::vec3(0,1,0)  // up
			);


		res_loc = glGetUniformLocation(prog->program_id, "view");
		glUniformMatrix4fv(res_loc, 1, GL_FALSE, &view[0][0]);

		for (long i = 0; i < planets.size(); ++i)
		{
			model = glm::translate(glm::mat4(1.0f), positions[i]);
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

	cam = {15, 15, 15};
	look_dir = glm::normalize(cam * -1.0f);
}
