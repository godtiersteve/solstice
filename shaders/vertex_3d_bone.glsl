#version 400
const int MAX_JOINTS = 100;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in ivec3 jointIds;
layout(location = 5) in vec3 jointWeights;

layout (std140) uniform BoneData {
	mat4 bones[MAX_JOINTS];
} boneData;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 cameraPos;
out vec4 fragColor;
out vec3 norm;
out vec3 fragPos;
out vec3 viewPos;
out vec2 fragTex;
out mat4 camView;

void main() {	
	mat4 boneTransform = boneData.bones[jointIds.x] * jointWeights.x;
	boneTransform+= boneData.bones[jointIds.y] * jointWeights.y;		
	boneTransform+= boneData.bones[jointIds.z] * jointWeights.z;			
	vec4 totalLocalPos = boneTransform * vec4(position, 1.0);	
	vec4 totalNormal = boneTransform * vec4(normal, 1.0);
    
	gl_Position = projection * view * model * totalLocalPos;
	fragColor = color;
	norm = normalize(mat3(transpose(inverse(view * model))) * totalNormal.xyz);
	fragPos = vec3(view * model * vec4(position, 1.0f));
	fragTex = texCoord + 1.0f;
	viewPos = vec3(view * vec4(cameraPos, 1.0f));		
	camView = view;
}