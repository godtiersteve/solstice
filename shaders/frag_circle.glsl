#version 420

uniform float radius;
uniform vec4 color;
uniform vec2 center;
uniform vec2 resolution;

in vec4 pos;
out vec4 frag_color;


vec4 circle(vec2 uv, vec2 pos, float rad, vec3 col) {
	float d = length(uv-pos);
	float alpha;
    if(d > rad) {
        alpha = 0.0f;
    }
    else {
        alpha = color.a;
    }
	return vec4(col, alpha);
}
void main()
{
	vec2 uv = vec2(gl_FragCoord.x - resolution.x / 2, gl_FragCoord.y - resolution.y / 2);
	frag_color = circle(pos.xy, center, radius, color.xyz);	
}