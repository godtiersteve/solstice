#version 330
in vec4 fragColor;
in vec2 fragTex;

uniform sampler2D tex;
uniform float timer;

out vec4 color;

void main() {	
    vec2 uv = fragTex * 1.0 + vec2(timer * -0.05);
    uv.y += 0.01 * (sin(uv.x * 3.5 + timer * 0.35) + sin(uv.x * 4.8 + timer * 3.05) + sin(uv.x * 7.3 + timer * 0.45)) / 3.0;
    uv.x += 0.12 * (sin(uv.y * 4.0 + timer * 0.5) + sin(uv.y * 6.8 + timer * 0.75) + sin(uv.y * 11.3 + timer * 0.2)) / 3.0;
    uv.y += 0.12 * (sin(uv.x * 4.2 + timer * 0.64) + sin(uv.x * 6.3 + timer * 1.65) + sin(uv.x * 8.2 + timer * 0.45)) / 3.0;

    vec4 tex1 = texture2D(tex, uv * 1.0);
    vec4 tex2 = texture2D(tex, uv * 1.0 + vec2(0.2));

    vec3 blue = vec3(0, 0.5, 1.0);
	
	//if(hasTexture) {
		//color = vec4(blue + vec3(tex1.a * 0.5 - tex2.a * 0.02), fragColor.a);
	//}
	//else {
		//color = fragColor;
	//}	

	color = vec4(blue + vec3(tex1.a * 0.5 - tex2.a * 0.02), fragColor.a);
}
