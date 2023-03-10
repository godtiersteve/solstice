#version 420

layout(location = 0) in vec2 position;

uniform mat4 projection;
uniform mat4 translation;
uniform mat4 scale;

out vec4 pos;
void main()
{
    gl_Position = vec4(position, 0, 1) * scale * translation *  projection;
	pos = vec4(position, 0, 1) * scale * translation;
}