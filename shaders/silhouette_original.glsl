#version 420

in vec2 frag_tex;

uniform vec4 color;
uniform sampler2D sampler;
uniform bool usesTexture;

out vec4 frag_color;


void main()
{
	if(usesTexture) {
		frag_color = texture(sampler, vec2(frag_tex.x, -frag_tex.y)) * color;	
		frag_color.x = color.x;
		frag_color.y = color.y;
		frag_color.z = color.z;
	}
	else {
		frag_color = color;
	}
}