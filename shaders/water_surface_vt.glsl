#version 330

#define SCALE 8.5

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float timer;
uniform float strength;

out vec4 fragColor;
out vec2 fragTex;
float calculateSurface(float x, float z) {
    float y = 0;
    y += (sin(x * 1.0 / SCALE + timer * 1.0) + sin(x * 2.3 / SCALE + timer * 1.5) + sin(x * 3.3 / SCALE + timer * 0.4)) / 3.0;
    y += (sin(z * 0.2 / SCALE + timer * 1.8) + sin(z * 1.8 / SCALE + timer * 1.8) + sin(z * 2.8 / SCALE + timer * 0.8)) / 3.0;
	//y+= 100;
    return y;
}

void main()
{
    vec3 pos = position;
        
    pos.y += strength * calculateSurface(pos.x, pos.z);
    pos.y -= strength * calculateSurface(0.0, 0.0);
	
    gl_Position = projection * view * model * vec4(pos, 1.0f);
	fragColor = color;		
	fragTex = texCoord;
}