#version 420

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 pos;

out vec4 fragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 0.0f, 1);
	fragPos = view * vec4(position, 0.0f, 0.0f);
} 