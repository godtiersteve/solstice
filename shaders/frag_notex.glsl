#version 420

in vec2 frag_tex;
uniform vec4 color;

out vec4 frag_color;

void main()
{
	frag_color =  color;	
}