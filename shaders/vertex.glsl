#version 420

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 frag_color;

void main()
{
    gl_Position = vec4(position.x, position.y, 0, 1);
	frag_color = color;
}