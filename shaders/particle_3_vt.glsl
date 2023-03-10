#version 400
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in vec3 posOffset;
layout(location = 5) in vec3 velOffset;
layout(location = 6) in float partStartTime;
layout(location = 7) in vec3 initalVel;

out vec3 outPos;
out vec3 velocity;
out float startTime;
out vec4 fragColor;
out vec2 fragTex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 gravity;
uniform float time;
uniform float H;
uniform float particleLifetime;

subroutine(RenderPassType)
void update() {
	//pos = position;
	//velocity = vertexVelocity;
	//startTime = vertexStartTime;
	//if(time >= startTime) {
		//float age = time - startTime;
		//if(age > particleLifetime) {
			//pos = vec3(0.0);
			//velocity = vertexInitialVelocity;
			//startTime = time;
		//} else {
//			pos+= velocity * H;
			//velocity+= accel * H;
		//}
	//}	
	//outPos = posOffset;
	//velocity = velOffset;
	//startTime = partStartTime;
	//outPos = posOffset * time;
	//velocity = posOffset * time;
	//startTime = 10.0f;
	outPos = initalVel;
	velocity = velOffset;
	startTime = 1.0f;
	//outPos = posOffset * time;
}
subroutine(RenderPassType)
void render() {
	vec3 pos = position + posOffset;
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	fragColor = vec4(0.0f, 1.0f, 0.5f, 1.0f);	
	fragTex = texCoord;
}
void main() {    
	RenderPass();
}


