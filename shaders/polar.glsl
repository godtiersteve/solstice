#define PI 3.14159265358979323844

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy - vec2(0.5,0.5);
	const float rInner = 0.25;
    const float rOuter = 0.5;
    
    float radius = length(uv);
    float angle = atan(uv.y, uv.x);
    
    vec2 coordPolar;
    coordPolar.s = (radius - rInner) / (rOuter - rInner);
    coordPolar.t = angle * 0.05 / PI + 0.5;
    fragColor = texture2D(iChannel0, coordPolar);
    
}