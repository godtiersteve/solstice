#version 400
subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexVelocity;
layout (location = 2) in float vertexStartTime;
layout (location = 3) in vec3 vertexInitialVelocity;

out vec3 position;
out vec3 velocity;
out float startTime;
out float transp;

uniform float time;
uniform float H;
uniform vec3 accel;
uniform float particleLifetime;

uniform mat4 mvp;

subroutine(RenderPassType)
void update() {
	position = vertexPosition;
	velocity = vertexVelocity;
	startTime = vertexStartTime;
	if(time >= startTime) {
		float age = time - startTime;
		if(age > particleLifetime) {
			position = vec3(0.0);
			velocity = vertexInitialVelocity;
			startTime = time;
		} else {
			position+= velocity * H;
			velocity+= accel * H;
		}
	}		
}

subroutine(RenderPassType)
void render() {
	float age = time - vertexStartTime;
	transp = 1.0f - age / particleLifetime;
	if(age < 0) {
		transp = 0.0f;
	}
	else {
		gl_PointSize*= 1.0f - age / 50.0f;
	}
	transp = 1.0f;
	gl_Position = mvp * vec4(vertexPosition, 1.0f);
	//gl_Position = mvp * vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
void main() {
	gl_PointSize = 30.0f;
	RenderPass();
}
