#version 420

in vec4 fragColor;
in vec2 texCoords;
flat in int fragTexID;
out vec4 color;

uniform sampler2DArray textures;

void main() {			
	if(fragTexID < 0) {
		color = fragColor;
	}
	else {	
		color = texture(textures, vec3(texCoords, fragTexID)) * fragColor;
		//color = fragColor;
	}	
}