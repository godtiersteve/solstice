#version 400

uniform vec4 outlineColor;
uniform sampler2D tex;
in vec4 fragCol;
in vec2 fragTex;
out vec4 color;
void main() {		
	if(outlineColor.a != 0.0) {
		color = outlineColor;
	}
	else {
		vec4 celColor = vec4(fragCol.rgb * 0.2, fragCol.a);
		if(fragTex.x < -1000) {
			color = celColor;		
		}
		else {			
			color = celColor * texture(tex, vec2(fragTex.x, -fragTex.y));
		}
	}
	color.a = 1.0;
}
