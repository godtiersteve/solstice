#version 420

in vec2 frag_tex;

uniform vec4 color;
uniform sampler2D sampler;
uniform bool usesTexture;

out vec4 frag_color;


void main()
{
	vec4 col = vec4(max(color.b, max(color.r, color.g)));
	if(usesTexture) {
		col = texture(sampler, vec2(frag_tex.x, -frag_tex.y)) * col;
	}
	else {
		col = color;
	}
	vec3 lum = vec3(0.299f, 0.587f, 0.144f);
	frag_color = vec4(vec3(dot(col.rgb, lum)), col.a);
}