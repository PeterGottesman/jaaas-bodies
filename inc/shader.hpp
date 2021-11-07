#pragma once
#include <string>

/* struct shader; */
/* struct program; */

enum shader_type {
	SHADER_FRAG,
	SHADER_VERT,
	SHADER_GEOMETRY,
	SHADER_COMPUTE,
	SHADER_TESS_CONTROL,
	SHADER_TESS_EVALUATION,

	SHADER_MAX
};

struct shader {
	enum shader_type type;
	int shader_id;
	char *filename;
};

struct program {
	int program_id;
	struct shader **shaders;
};


struct shader *shader_load(std::string filename, enum shader_type);
/* int shader_attach(struct shader *shader, struct program *program); */
void shader_unload(struct shader *shader);

struct program *program_load(struct shader **shaders, int count);
void program_unload(struct program *program);
