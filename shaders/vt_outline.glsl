#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float outlineWidth;
uniform vec3 camera;

out vec4 fragCol;
out vec2 fragTex;
out vec3 norm;
out vec3 fragCamera;
out vec3 fragPos;
out float thickness;

void main()
{	
	vec3 pos = position + normalize(normal) * outlineWidth;
    gl_Position = projection * view * model * vec4(pos, 1.0f);
	fragCol = color;
	fragTex = texCoord;
	norm = normalize(mat3(transpose(inverse(view * model))) * normal);	
	fragCamera = vec3(view * vec4(camera, 1.0f));	
	fragPos = vec3(view * model * vec4(pos, 1.0f));
	thickness = outlineWidth;
}

