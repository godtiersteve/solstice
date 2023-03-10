#version 420

out vec4 fragColor;
in vec4 color;
in vec2 clipOffset;
flat in int textureID;
uniform vec2 clipSize;
uniform sampler2DArray textures;

void main() {			
	if(color.a < 0.01) {
		discard;
	}
	vec2 texCoord = gl_PointCoord;
	texCoord.x*= clipSize.x;
	texCoord.y*= clipSize.y;
	mat3 trans = mat3(vec3(1, 0, clipOffset.x * clipSize.x),
					  vec3(0, 1, clipOffset.y * clipSize.y),
					  vec3(0, 0, 1));
	texCoord = vec2(vec3(texCoord, 1) * trans);
	fragColor = texture(textures, vec3(gl_PointCoord, textureID)) * color;				
	//fragColor = vec4(1.0);
}