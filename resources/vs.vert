#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

out vec3 fc;
out vec3 norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);

	norm = (model * vec4(normal, 0.0)).xyz;
	fc = color;
}
