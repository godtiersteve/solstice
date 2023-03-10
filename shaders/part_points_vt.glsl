#version 420
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 col;
layout (location = 2) in float scale;
layout (location = 3) in vec2 clipCoord;
layout (location = 4) in int texID;

out vec2 clipOffset;
out vec4 color;
uniform mat4 mvp;
flat out int textureID;

void main() {
	color = col;
	clipOffset = clipCoord;
	textureID = texID;
	gl_PointSize = scale;
	gl_Position = mvp * vec4(position, 1.0f);
}
