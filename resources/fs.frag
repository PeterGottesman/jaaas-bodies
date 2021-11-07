#version 330 core

uniform vec3 cam_dir;

in vec3 fc;
in vec3 norm;

out vec4 FragColor;

void main()
{
	FragColor = vec4(fc, 0.0) * dot(cam_dir, -norm);
}
