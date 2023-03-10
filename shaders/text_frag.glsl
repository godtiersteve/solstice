#version 330

in vec2 fragPos;
in vec2 frag_tex;
out vec4 frag_color;

uniform vec4 color;
uniform sampler2D sampler;

void main()
{
	vec4 sampled = vec4(color.rgb, texture(sampler, vec2(frag_tex.x, -frag_tex.y)).r);
	frag_color = (color.rgb, 1.0) * sampled;
	//frag_color = vec4(fragPos, 0, 1.0);	
}