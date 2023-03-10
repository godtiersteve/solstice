#version 400

in float transp;
uniform sampler2D particleTex;
layout (location = 0) out vec4 fragColor;
void main() {
	//fragColor = texture(particleTex, gl_PointCoord);
	//fragColor.a*= transp;
	fragColor = texture(particleTex, gl_PointCoord) * vec4(0.0f, 0.0f, 1.0f, 1.0);
	//fragColor = vec4(gl_PointCoord.st, 1.0f, 1.0f);
	
}