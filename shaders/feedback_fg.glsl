#version 400
uniform sampler2D particleTex;
in float transp;
layout (location = 0 ) out vec4 fragColor;

void main() {
	fragColor = texture(particleTex, gl_PointCoord);
	fragColor.a*= transp;
}