#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float outlineWidth;

void main()
{	
	vec3 norm = normalize(mat3(transpose(inverse(view * model))) * normal);
	vec3 pos = position + normalize(normal) * outlineWidth;
    gl_Position = projection * view * model * vec4(position, 1.0f) + vec4(norm * outlineWidth, 0.0f);
}