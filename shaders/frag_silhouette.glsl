#version 420

in vec2 frag_tex;

uniform vec4 color;
uniform sampler2D sampler;

out vec4 frag_color;

void main() {
	frag_color = texture(sampler, vec2(frag_tex.x, -frag_tex.y)) * color;
	frag_color = vec4(color.rgb, frag_color.a);
}