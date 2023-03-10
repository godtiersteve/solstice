#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

out vec2 uv;
void main()
{
	gl_Position = vec4(position, 1.0);
	uv = texCoord;
}