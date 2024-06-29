#version 330 core
out vec4 FragColor;

in vec3 Pos;

void main()
{
	FragColor = vec4(Pos.x, Pos.y, Pos.z, 1.0f);
}