#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec3 lightPos;
in vec2 fragTex;

uniform sampler2D tex;
uniform bool hasTexture;
uniform bool noLight;

out vec4 color;
void main() {
	if(hasTexture) {
		color = fragColor * texture(tex, vec2(fragTex.x, -fragTex.y));
	}
	else {
		color = fragColor;
	}
}
