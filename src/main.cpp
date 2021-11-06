#include <stdio.h>
#include <stdlib.h>

#include "app.hpp"
#include "shader.hpp"
int width, height;

static char *load_file(char *filename, int *length)
{
	FILE *f;
	char *output;

	f = fopen(filename, "r");
	if (f == NULL)
	{
		perror("load_file");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*length = ftell(f);

	output = (char *)malloc(*length);

	rewind(f);

	fread(output, 1, *length, f);
	fclose(f);

	return output;
}

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

int main(int argc, char *argv[])
{
	GLFWwindow *win;
	int res_loc;
	unsigned int vert_buf, elem_buf;
	float verts[] = {
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f,
	};
	unsigned int tris[] = {
		0, 1, 2,
		2, 1, 3
	};

	glfwInit();

	width = height = 400;

	win = glfwCreateWindow(width, height, "Window Title", NULL, NULL);
	glfwSetKeyCallback(win, keyhandler);

	if (!win)
	{
		fprintf(stderr, "Failed to create window\n");
		return -1;
	}

	glfwGetWindowSize(win, &width, &height);

	/* Make opengl context the active context for this thread */
	glfwMakeContextCurrent(win);
	glewInit();

	glViewport(0, 0, width, height);

	/* Generate and bind VAO */
	glGenVertexArrays(1, &vert_buf);
	glBindVertexArray(vert_buf);

	glBindBuffer(GL_ARRAY_BUFFER, vert_buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &elem_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elem_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tris), tris, GL_STATIC_DRAW);

	struct program *prog = loadShaders();

	res_loc = glGetUniformLocation(prog->program_id, "uResolution");
	glUniform2f(res_loc, width, height);

	while (!glfwWindowShouldClose(win))
	{
		glClearColor(0.05, 0, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		/* glDrawArrays(GL_TRIANGLES, 0, 3); */
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(win);
	}

	glfwDestroyWindow(win);

	return 0;
}

