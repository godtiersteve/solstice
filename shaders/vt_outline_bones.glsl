#version 400
const int MAX_JOINTS = 100;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in ivec3 jointIds;
layout(location = 5) in vec3 jointWeights;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
layout (std140) uniform BoneData {
	mat4 bones[MAX_JOINTS];
} boneData;

uniform float outlineWidth;

out vec4 fragCol;
out vec2 fragTex;
out vec3 fragNorm;
out vec3 fragPos;
void main() {	
	mat4 boneTransform = boneData.bones[jointIds.x] * jointWeights.x;
	boneTransform+= boneData.bones[jointIds.y] * jointWeights.y;		
	boneTransform+= boneData.bones[jointIds.z] * jointWeights.z;			
	vec4 totalLocalPos = boneTransform * vec4(position, 1.0);	
	vec4 totalNormal = boneTransform * vec4(normal, 1.0);
	vec3 pos = totalLocalPos.xyz + normalize(totalNormal.xyz) * outlineWidth;	
	//vec3 pos = totalLocalPos.xyz * 1.5;	
	gl_Position = projection * view * model * vec4(pos, 1.0);	
	fragCol = color;
	fragTex = texCoord;
	fragNorm = normalize(mat3(transpose(inverse(view * model))) * totalNormal.xyz);
	fragPos = vec3(view * model * vec4(pos, 1.0f));
}

