#version 400
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec2 fragTex;
in vec3 viewPos;
in mat4 camView;

uniform sampler2D tex;
uniform sampler2D normTex;
uniform sampler2D renderPos;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec4 direction;
	float spotExponent;
};
uniform Light lights[20];
uniform int numLights;
uniform int width;
uniform int height;
uniform float edgeThreshold;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNorm;

float stepmix(float edge0, float edge1, float E, float x)  {
	float t = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
	return mix(edge0, edge1, t);
}
float CalcDiffuse(float df) {
	const float A = 0.1f;
	const float B = 0.8f;
	const float C = 0.8f;
	const float D = 0.8f;
	
	float E = fwidth(df);
	if(df > A - E && df < A + E) {
		df = stepmix(A, B, E, df);
	}
	else if(df > B - E && df < B + E) {
		df = stepmix(B, C, E, df);
	}
	else if(df > C - E && df < C + E) {
		df = stepmix(C, D, E, df);
	}	
	else if(df < A) {
		df = 0.0f;
	}
	else if(df < B) {
		df = B;
	}
	else if(df < C) {
		df = C;
	}
	else {
		df = D;
	}
	return df;
}
float CalcSpec(float spec) {
	float E = fwidth(spec);
	if(spec > 0.5 - E && spec < 0.5 + E) {
		spec = clamp(0.25 * (spec - 0.5 + E) / E, 0.0, 1.0);
	}
	else {
		spec = step(0.5f, spec);
	}
	return spec;
}
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {	
	vec3 lightDir = normalize(-light.direction.xyz);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0));
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));			
	float attenunation = 1.0f;	
	return (light.ambient + light.diffuse * diff + light.specular * spec);
}
vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir, vec3 pos) {
	vec3 positionToLightSource = light.position - pos;
	vec3 lightDir = normalize(positionToLightSource);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0f));
	vec3 reflectDir = reflect(-lightDir, normal);	
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));		
	float dist = length(light.position - pos);
	float attenunation = 1.0 / (light.direction.x + light.direction.y * dist + light.direction.z * dist * dist);	
	if(light.direction.w <= 90) {
		float clampedCosine = max(0.0, dot(-lightDir, normalize(light.direction.xyz)));
		if(clampedCosine < light.direction.w) {
			attenunation = 0.0f;
		}
		else {
			attenunation = attenunation * pow(clampedCosine, light.spotExponent);
		}
	}
	//float attenunation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);	
	return (light.ambient + light.diffuse * diff + light.specular * spec) * attenunation;
}
float luma(vec3 color) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}
vec4 ApplyLighting(vec3 normal, vec3 pos) {	
	vec4 celColor;	
	vec3 viewDir = normalize(viewPos - pos);		
	if(fragColor.a < 0) {
		celColor = fragColor;
	}
	else {
		for(int i = 0; i < numLights; i++) {		
			Light light = lights[i];	
			light.position = vec3(camView * vec4(light.position, 1.0));		
			if(light.direction.w > 0) {
				float w = light.direction.w;
				light.direction.w = 1.0;
				light.direction = transpose(inverse(camView)) * light.direction;
				light.direction.w = w;
				celColor+= vec4(CalcDirLight(light, normal, viewDir), 0.0f);	
			}
			else {
				celColor+= vec4(CalcPointLight(light, normal, viewDir, pos), 0.0f);	
			}
		}
		celColor.a = fragColor.a;
	}
	//return celColor * texture(tex, vec2(fragTex.x, -fragTex.y)) * fragColor;		
	return celColor * texture(tex, fragTex) * fragColor;		
}

vec4 celOutline() {
	/*
	float dx = 1.0f / float(width);
	float dy = 1.0f / float(height);
	vec3 center = texture(normTex, fragTex + vec2(0.0, 0.0)).rgb;
	vec3 top = texture(normTex, fragTex + vec2(0.0, dy)).rgb;
	vec3 topRight = texture(normTex, fragTex + vec2(dx, dy)).rgb;
	vec3 right = texture(normTex, fragTex + vec2(dx, 0.0)).rgb;
	
	vec3 t = center - top;
	vec3 r = center - right;
	vec3 tr = center - topRight;
	t = abs(t);
	r = abs(r);
	tr = abs(tr);
	float n = 0.0;
	n = max(n, t.x);
	n = max(n, t.y);
	n = max(n, t.z);
	n = max(n, r.x);
	n = max(n, r.y);
	n = max(n, r.z);
	n = max(n, tr.x);
	n = max(n, tr.y);
	n = max(n, tr.z);
	
	n = 1.0 - clamp(clamp((n * 10.0) - 0.8, 0.0, 1.0) * 2.5, 0.0, 1.0);
	
	return vec4(texture(tex, fragTex).rgb * (0.5 + 0.9 * n), 1.0);
	*/
	/*
	float dx = 1.0 / float(width);
	float dy = 1.0 / float(height);
	float s00 = luma(texture(normTex,
					 fragTex + vec2(-dx,dy) ).rgb);
	float s10 = luma(texture(normTex,
					 fragTex + vec2(-dx,0.0) ).rgb);
	float s20 = luma(texture(normTex,
					 fragTex + vec2(-dx,-dy) ).rgb);
	float s01 = luma(texture(normTex,
					 fragTex + vec2(0.0,dy) ).rgb);
	float s21 = luma(texture(normTex,
					 fragTex + vec2(0.0,-dy) ).rgb);
	float s02 = luma(texture(normTex,	
					 fragTex + vec2(dx, dy) ).rgb);
	float s12 = luma(texture(normTex,
					 fragTex + vec2(dx, 0.0) ).rgb);
	float s22 = luma(texture(normTex,
					 fragTex + vec2(dx, -dy) ).rgb);
	float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
	float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
	float dist = sx * sx + sy * sy;
	if(dist > 0.25f)
		return vec4(0.0, 0.0, 0.0, 1.0);
	else
		return ApplyLighting(normalize(texture(normTex, fragTex).xyz), fragPos);
	*/
	return vec4(0.5f);
}

void main() {	
	//outColor = texture(renderPos, fragTex);
	//outColor = texture(normTex, fragTex);
	//outColor = texture(tex, fragTex) * fragColor;
	//outColor = ApplyLighting(normalize(texture(normTex, fragTex).xyz), texture(renderPos, fragTex).xyz);
	float dx = 1.0f / float(width);
	float dy = 1.0f / float(height);
	vec3 center = texture(normTex, fragTex + vec2(0.0, 0.0)).rgb;
	vec3 top = texture(normTex, fragTex + vec2(0.0, dy)).rgb;
	vec3 topRight = texture(normTex, fragTex + vec2(dx, dy)).rgb;
	vec3 right = texture(normTex, fragTex + vec2(dx, 0.0)).rgb;
	
	vec3 t = center - top;
	vec3 r = center - right;
	vec3 tr = center - topRight;
	t = abs(t);
	r = abs(r);
	tr = abs(tr);
	float n = 0.0;
	n = max(n, t.x);
	n = max(n, t.y);
	n = max(n, t.z);
	n = max(n, r.x);
	n = max(n, r.y);
	n = max(n, r.z);
	n = max(n, tr.x);
	n = max(n, tr.y);
	n = max(n, tr.z);
	
	n = 1.0 - clamp(clamp((n * 10.0) - 0.8, 0.0, 1.0) * 2.5, 0.0, 1.0);
	vec4 finalColor = ApplyLighting(normalize(texture(normTex, fragTex).xyz), texture(renderPos, fragTex).xyz);
	outColor = vec4(finalColor.rgb* (0.25 + 0.5 * n), finalColor.a);
}
