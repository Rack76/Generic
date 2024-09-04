#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 perspective = mat4(1.0);

void main()
{
	gl_Position = vec4(position, 1.0) * model * view * perspective;
	fTexCoord = vTexCoord;
}