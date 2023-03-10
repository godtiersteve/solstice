#version 420

in vec2 frag_tex;

uniform vec4 color;
uniform sampler2D sampler;
uniform bool usesTexture;

out vec4 frag_color;


void main()
{
	if(usesTexture) {
		frag_color = texture(sampler, vec2(frag_tex.s, -frag_tex.t)) * color;	
	}
	else {
		vec2 circleCheck = vec2(frag_tex.s, frag_tex.t);
		if(circleCheck.s * circleCheck.s + circleCheck.t * circleCheck.t >= 1.0 ||
		   circleCheck.s * circleCheck.s + circleCheck.t * circleCheck.t <= 0.5) {
			frag_color = vec4(0);
		}
		else {	
			frag_color = color;
		}
	}
}