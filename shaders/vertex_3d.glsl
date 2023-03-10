#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform vec3 cameraPos;

out vec4 fragColor;
out vec3 norm;
out vec3 fragPos;
out vec3 viewPos;
out vec2 fragTex;
out mat4 camView;
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	fragColor = color;	
	norm = normalize(mat3(transpose(inverse(view * model))) * normal);
	fragPos = vec3(view * model * vec4(position, 1.0f));
	viewPos = vec3(view * vec4(cameraPos, 1.0f));	
	fragTex = texCoord;
	camView = view;
}