#version 420

in vec4 fragColor;
in vec2 fragTex;
in vec3 fragPos;
out vec4 color;

uniform sampler2D tex;

void main() {			
	color = texture(tex, vec2(fragTex.x, -fragTex.y)) * fragColor;
	//color = texture(tex, vec2(texCoords.x, -texCoords.y));
	//color = vec4(1.0f, 0.0f, 0.0f, 0.25f);
}