#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;

out vec3 fc;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * model * view * vec4(aPos, 1.0);
	fc = color;
}
