#version 330 core
out vec4 FragColor;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(0.6, 0.9, 0.34, 1.0);
}