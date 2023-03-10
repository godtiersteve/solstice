#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 col;

out vec2 fragPos;
out vec2 texCoord;
out vec4 fragColor;
void main()
{
    gl_Position = vec4(position.xy, 0, 1);
	fragPos = position;
	texCoord = texcoord;
	fragColor = col;
} 