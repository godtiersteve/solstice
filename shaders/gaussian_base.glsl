#version 420

in vec2 frag_tex;

uniform vec4 color;
uniform sampler2D sampler;
uniform float radius;
uniform float resolution;
uniform vec2 dir;
out vec4 frag_color;


void main() {
	vec4 sum = vec4(0.0f);
	vec2 tc = frag_tex;
	float blur = radius / resolution;
		
	float hstep = dir.x;
	float vstep = dir.y;
	
    sum += texture2D(sampler, vec2(tc.x - 4.0 * blur * hstep, tc.y - 4.0 *blur * vstep)) * 0.0162162162;
    sum += texture2D(sampler, vec2(tc.x - 3.0 * blur * hstep, tc.y - 3.0 *blur * vstep)) * 0.0540540541;
    sum += texture2D(sampler, vec2(tc.x - 2.0 * blur * hstep, tc.y - 2.0 *blur * vstep)) * 0.1216216216;
    sum += texture2D(sampler, vec2(tc.x - 1.0 * blur * hstep, tc.y - 1.0 *blur * vstep)) * 0.1945945946;
	
	sum += texture2D(sampler, vec2(tc.x, tc.y)) * 0.2270270270;
	
    sum += texture2D(sampler, vec2(tc.x + 1.0 * blur * hstep, tc.y + 1.0 * blur * vstep)) * 0.1945945946;
    sum += texture2D(sampler, vec2(tc.x + 2.0 * blur * hstep, tc.y + 2.0 * blur * vstep)) * 0.1216216216;
    sum += texture2D(sampler, vec2(tc.x + 3.0 * blur * hstep, tc.y + 3.0 * blur * vstep)) * 0.0540540541;
    sum += texture2D(sampler, vec2(tc.x + 4.0 * blur * hstep, tc.y + 4.0 * blur * vstep)) * 0.0162162162;
	
	frag_color = color * sum;
}

void main() {
	float[5] offset = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
	float[5] weight = float[]( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );
	frag_color = texture2D(sampler, vec2(frag_tex.x / imageSize.x, frag_tex.y /imageSize.y));
	for(int i = 1; i < 5; i++) {
		frag_color+= texture2D(sampler, vec2(frag_tex.x, -frag_tex.y) + vec2(0.0f, offset[i]) / imageSize.y) * weight[i];
		frag_color+= texture2D(sampler, vec2(frag_tex.x, -frag_tex.y) - vec2(0.0f, offset[i]) / imageSize.y) * weight[i];
	}	

	frag_color = vec4(color.rgb, frag_color.a);
	//texture(sampler, vec2(frag_tex.x, frag_tex.y)) * color;	
	//frag_color = vec4(color.rgb, sum.a * 2);
}