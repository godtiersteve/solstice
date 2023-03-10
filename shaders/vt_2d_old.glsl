#version 420

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in int texID;

uniform mat4 mv;
uniform mat4 proj;
uniform vec2 norm;

out vec4 fragColor;
out vec2 texCoords;
flat out int fragTexID;

void main() {
	vec2 pos = position;
	pos.x*= norm.x;
	pos.y*= norm.y;
	fragTexID = texID;
    gl_Position = proj * mv * vec4(pos, 0.0f, 1.0f);	
	fragColor = color;
	texCoords = texCoord;
} 
