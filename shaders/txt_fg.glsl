#version 420

uniform vec4 fragColor;
in vec2 texCoords;
in vec3 fragPos;
out vec4 color; 

uniform sampler2D tex;

void main() {			
	vec4 sampled = vec4(fragColor.rgb, texture(tex, vec2(texCoords.x, -texCoords.y)).r);
	color = (color.rgb, 1.0) * sampled;
}