#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "shader.hpp"
#include "fileio.hpp"

static inline int type2gl(enum shader_type type)
{
	switch (type)
	{
		case SHADER_FRAG:
			return GL_FRAGMENT_SHADER;
		case SHADER_VERT:
			return GL_VERTEX_SHADER;
		case SHADER_GEOMETRY:
			return GL_GEOMETRY_SHADER;
		case SHADER_COMPUTE:
			return GL_COMPUTE_SHADER;
		case SHADER_TESS_CONTROL:
			return GL_TESS_CONTROL_SHADER;
		case SHADER_TESS_EVALUATION:
			return GL_TESS_EVALUATION_SHADER;
		default:
			return -1;
	}
}

struct shader *shader_load(std::string filename, enum shader_type type)
{
	struct shader *shader;
	char infoLog[512];
	int s_len, s_id;
	char *source;
	int success;

	source = load_file(filename, &s_len);
	if (source == nullptr)
	{
		return nullptr;
	}

	s_id = glCreateShader(type2gl(type));
	glShaderSource(s_id, 1, (const char **)&source, &s_len);

	glCompileShader(s_id);

	glGetShaderiv(s_id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(s_id, 512, NULL, infoLog);
		printf("Error compiling shader %s:\n %s\n", filename.c_str(), infoLog);
		return NULL;
	}

	// shader = malloc(sizeof(struct shader));
	shader = new struct shader();
	shader->type = type;
	shader->shader_id = s_id;
	shader->filename = strdup(filename.c_str());

	return shader;
}

void shader_unload(struct shader *shader)
{
	if (shader == NULL)
		return;

	glDeleteShader(shader->shader_id);
	free(shader->filename);
	free(shader);
}

struct program *program_load(struct shader **shaders, int count)
{
	struct program *prog;
	int prog_id;

	prog_id = glCreateProgram();
	for (int i = 0; i < count; ++i)
		glAttachShader(prog_id, shaders[i]->shader_id);

	glLinkProgram(prog_id);
	glUseProgram(prog_id);

	// prog = malloc(sizeof(struct program));
	// prog->shaders = malloc(sizeof(struct shader *) * count);
	prog = new struct program();
	prog->shaders = new struct shader *[count];
	prog->program_id = prog_id;

	for (int i = 0; i < count; ++i)
		prog->shaders[i] = shaders[i];

	return prog;
}

void program_unload(struct program *prog)
{
	if (prog == NULL)
		return;

	glDeleteProgram(prog->program_id);

	free(prog->shaders);
	free(prog);
}
