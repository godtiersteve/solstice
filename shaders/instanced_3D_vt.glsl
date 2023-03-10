#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in vec3 initialVel;
layout(location = 5) in float startTime;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 gravity;
uniform float time;
uniform float particleLifetime;

out vec4 fragColor;
out vec2 fragTex;
void main() {
	vec3 pos = position;
	if(time > startTime) {	
		float t = time - startTime;
		if(t < particleLifetime) {
			pos = position + initialVel * t + gravity * t * t;
		}
	}
    gl_Position = projection * view * model * vec4(pos, 1.0f);
	fragColor = vec4(0.0f, 1.0f, 0.5f, 1.0f);	
	fragTex = texCoord;
}