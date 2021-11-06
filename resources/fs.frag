#version 330 core

uniform vec2 uResolution;
out vec4 FragColor;

void main()
{
	vec2 iCoord = gl_FragCoord.xy/uResolution;
	FragColor = vec4(iCoord, 0.0, 0.0);
}
