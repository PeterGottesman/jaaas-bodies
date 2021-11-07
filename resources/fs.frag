#version 330 core

uniform vec2 uResolution;
in vec3 fc;

out vec4 FragColor;

void main()
{
	// vec2 iCoord = gl_FragCoord.xy/uResolution;
	FragColor = vec4(fc, 0.0);
}
