#version 420

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec4 texOffset;
layout(location = 4) in vec4 posOffset;
layout(location = 5) in int texID;
layout(location = 6) in float angle;

uniform mat4 mv;
uniform mat4 proj;
uniform vec2 norm;

out vec4 fragColor;
out vec2 texCoords;
flat out int fragTexID;

void main() {
	vec2 pos = position;	
	mat3 trans = mat3(1.0, 0.0, posOffset.x,
					  0.0, 1.0, posOffset.y,
				     0.0, 0.0, 1.0);
	mat3 scale = mat3(posOffset.z, 0.0, 0.0,
						0.0, posOffset.w, 0.0,
						0.0, 0.0, 1.0);						
	if(angle != 0.0) {
		mat3 rot = mat3(cos(angle), -sin(angle), 0.0,
					sin(angle), cos(angle), 0.0,
					0.0, 0.0, 1.0);
		trans = scale * rot * trans;
	}
	else {
		trans = scale * trans;
	}
	pos = (vec3(pos, 1.0) * trans).xy;
	pos.x*= norm.x;
	pos.y*= norm.y;

    gl_Position = proj * mv * vec4(pos, 0.0f, 1.0f);
	fragColor = color;
	mat3 texTrans = mat3(texOffset.z, 0.0, texOffset.x,
						0.0, texOffset.w, texOffset.y,
						0.0, 0.0, 1.0);
	texCoords = (vec3(texCoord, 1.0) * texTrans).xy;
	fragTexID = texID;
} 
