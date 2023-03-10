#version 330

uniform vec4 outlineColor;
uniform sampler2D tex;

in vec4 fragCol;
in vec2 fragTex;
in vec3 norm;
in vec3 fragCamera;
in vec3 fragPos;
in float thickness;

out vec4 color;

void main() {	
	vec4 celColor = texture(tex, vec2(fragTex.x, -fragTex.y));
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 viewDir = -normalize(fragCamera - fragPos);
	float nDotL = max(dot(norm, viewDir), 0.0);
	color = nDotL *  celColor * vec4(0.0, 0.0, 0.0, 1.0);
}
