#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec2 cameraPos;

out vec4 fragColor;
out vec3 Normal;
out vec3 fragPos;
out vec3 lightPos;
out vec2 fragTex;
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	fragColor = color;	
	Normal = mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(model * vec4(position, 1.0f));
	lightPos = vec3(cameraPos.x, cameraPos.y, -5.0f);
	fragTex = texCoord;
}