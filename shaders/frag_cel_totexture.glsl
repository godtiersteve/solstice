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
	float spotCutoff;
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
	const float A = 0.3f;
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
vec3 CalcLight(Light light, vec3 normal, vec3 viewDir) {
	float attenuation = 1.0;
	vec3 lightDirection = vec3(0.0);
	if(light.direction.w == 0.0) {
		lightDirection = normalize(-light.direction.xyz);
		attenuation = 1.0;
	}
	else if(light.direction.w != 0.0 && light.spotCutoff > 90) {
		vec3 positionToLightSource = light.position - fragPos;
		float distance = length(positionToLightSource);
		lightDirection = normalize(positionToLightSource);
	}
	else if(light.direction.w != 0.0 && light.spotCutoff <= 90.0) {
		vec3 positionToLightSource = light.position - fragPos;
		float distance = length(positionToLightSource);
		attenuation = 1.0 / distance;
		lightDirection = normalize(positionToLightSource);
		
		float clampedCosine = max(0.0, dot(-lightDirection, light.direction.xyz));
		float cosCutOff = cos(light.spotCutoff);
		if(clampedCosine < cosCutOff) {
			attenuation = 0.0;
		}
		else {
			attenuation = attenuation * pow(clampedCosine, light.spotExponent);
		}
	}
	float diff = CalcDiffuse(max(dot(normal, lightDirection), 0.0f));
	vec3 reflectDir = reflect(-lightDirection, normal);	
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));		
	return (light.ambient + light.diffuse * diff + light.specular * spec) * attenuation;
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
			celColor+= vec4(CalcLight(light, normal, viewDir), 0.0f);	
		}
		celColor.a = fragColor.a;
	}
	
	return celColor * texture(tex, vec2(fragTex.x, fragTex.y)) * fragColor;	
}

void main() {			
	outColor = ApplyLighting(normalize(texture(normTex, fragTex).xyz), texture(renderPos, fragTex).xyz);	
	//outColor = texture(renderPos, fragTex);
	//outColor = texture(normTex, fragTex);
	//outColor = texture(tex, fragTex) * fragColor;
}
