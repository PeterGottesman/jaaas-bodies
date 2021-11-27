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

		case GLFW_KEY_R:
			a->rotate_cam(DIR_UP);
			break;
		case GLFW_KEY_F:
			a->rotate_cam(DIR_DOWN);
			break;

		case GLFW_KEY_Q:
			a->rotate_cam(DIR_LEFT);
			break;
		case GLFW_KEY_E:
			a->rotate_cam(DIR_RIGHT);
			break;

		case GLFW_KEY_W:
			a->move_cam(DIR_FRONT);
			break;
		case GLFW_KEY_A:
			a->move_cam(DIR_LEFT);
			break;
		case GLFW_KEY_S:
			a->move_cam(DIR_BACK);
			break;
		case GLFW_KEY_D:
			a->move_cam(DIR_RIGHT);
			break;

		case GLFW_KEY_G:
			a->move_cam(DIR_UP);
			break;
		case GLFW_KEY_T:
			a->move_cam(DIR_DOWN);
			break;
		default:
			break;
	}
}

void App::move_cam(direction dir)
{
	switch (dir)
	{
		case DIR_UP:
			cam.y += 1.0;
			break;
		case DIR_DOWN:
			cam.y -= 1.0;
			break;
		case DIR_RIGHT:
			cam += glm::cross(look_dir, {0.0f, 1.0f, 0.0f});
			break;
		case DIR_LEFT:
			cam -= glm::cross(look_dir, {0.0f, 1.0f, 0.0f});
			break;
		case DIR_FRONT:
			cam += look_dir;
			break;
		case DIR_BACK:
			cam -= look_dir;
			break;
	}
}

void App::rotate_cam(direction axis)
{
	glm::vec3 rot_axis;
	switch (axis)
	{
		case DIR_UP:
			rot_axis = glm::cross(look_dir, {0, 1, 0});
			break;
		case DIR_DOWN:
			rot_axis = glm::cross(look_dir, {0, -1, 0});
			break;
		case DIR_LEFT:
			rot_axis = {0, 1, 0};
			break;
		case DIR_RIGHT:
			rot_axis = {0, -1, 0};
			break;
		default:
			return;
	}

	look_dir = glm::rotate(look_dir, glm::radians(1.0f), rot_axis);
}

void App::run()
{
	int res_loc;

	// std::vector<glm::fvec3> positions;
	std::vector<Planet> planets;

	for (int i = 0; i < sim->num_points; ++i)
	{
		planets.push_back({
				sim->body->unmoving[i],
				(float)sim->body->radius[i]
			});
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
		glClearColor(0.05, 0, 0.1, 1);
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

		for (unsigned long i = 0; i < planets.size(); ++i)
		{
			glm::fvec3 pos = {
				sim->body->x[i],
				sim->body->y[i],
				sim->body->z[i],
			};
			model = glm::translate(glm::mat4(1.0f), pos);
			model = glm::rotate(model, t, {0, 1, 0});
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

	cam = {125, 160, 150};
	look_dir = glm::normalize(cam * -1.0f);

	this->sim = sim;
}
