#version 330

uniform vec4 color;
uniform sampler2D tex;

in vec2 fragPos;
in vec2 texCoord;
in vec4 fragColor;

out vec4 frag_color;

void main() {		
	vec4 sampled = vec4(vec3(1.0f), texture(tex, vec2(texCoord.x, -texCoord.y)).r);
	frag_color = fragColor * sampled;
	//frag_color = texture(tex, vec2(texCoord.x, -texCoord.y));
	//frag_color = texture(tex, texCoord) * color;	
	//frag_color = vec4(1.0);
}