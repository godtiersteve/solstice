#version 420

in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec2 fragTex;

uniform sampler2D textures[20];

out vec4 color;

void main() {	
	if(fragTex.x < -100) {
		color = fragColor;		
	}
	else {	
		float xTex = mod(fragTex.x, 1000);
		int offset = int((fragTex.x - xTex) / 1000) - 1;	
		vec2 tex = vec2(xTex, -mod(fragTex.y, 1000));
		//color = texture(textures[offset], tex) * vec4(fragColor.xyz, 0.5f);
		color = texture(textures[offset], tex) * fragColor;
		//color = vec4(1.0f, 1.0f, 1.0f, 0.5f);
	}	
}
