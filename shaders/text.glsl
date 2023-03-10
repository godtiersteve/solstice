#version 400
layout(location = 0) in vec2 position;
//layout(location = 1) in vec4 color;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 posOffset;
layout(location = 3) in vec2 texOffset;

uniform mat4 mv;
uniform mat4 proj;
out vec4 fragColor;
out vec2 texCoords;
out vec3 fragPos;

void main() {
	float scale = texOffset.y;
	if(scale < 0.001f) {
		scale = 1.0f;
	}
	float x = position.x * scale + posOffset.x;
	float y = position.y + posOffset.y;
	gl_Position = proj * mv * vec4(x, y, 0.0f, 1.0f);
	fragPos = vec3(mv * vec4(position, 0.0f, 1.0f));	
	if(texOffset.y < -0.5f) {		
		fragColor.a = 0.0f;
	}	
	texCoords = texCoord + vec2(texOffset.x, 0.0f) ;
}
