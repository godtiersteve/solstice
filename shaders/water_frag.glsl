#version 420

uniform vec4 color;
uniform float time;

out vec4 frag_color;
in vec4 fragPos;

#define TAU 6.28318530718
#define MAX_ITER 5

void main()
{	
	vec4 col;
	vec2 uv = fragPos.xy / vec2(0.3, 0.3);
	//uv = clamp(uv, 0, 1);
	vec2 p = mod(uv + TAU, TAU) - 250.0;
	vec2 i = vec2(p);
	float c = 1.0f;
	float ttime = time * 0.025f;
	float inten = 0.005f;
	for(int n = 0; n < MAX_ITER; n++) {
		float t = ttime * (1.0f - (3.0f / (n + 1.0f)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c+= 1.0 / length(vec2(p.x / (sin(i.x + t) / inten), p.y / (cos(i.y + t) / inten)));		
	}
	c/= float(MAX_ITER);
	c = 1.17 - pow(c, 1.2);
	col = vec4(vec3(pow(abs(c), 8.0)), 0.0);
	col = clamp(col + color, 0.0, 1.0);
	col.a = color.a;

	frag_color = col;
}