#version 330
in vec4 fragColor;
in vec2 fragTex;
in vec3 norm;
in vec3 fragPos;
in vec3 lightPos;
in mat4 camView;

uniform sampler2D tex;
uniform float timer;
uniform bool noTex;

out vec4 color;
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec4 direction;
	float spotExponent;
	float spotCutoff;
};
uniform Light light;

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
void main() {	
    vec2 uv = fragTex * 1.0 + vec2(timer * -0.05);
    uv.y += 0.01 * (sin(uv.x * 3.5 + timer * 0.35) + sin(uv.x * 4.8 + timer * 3.05) + sin(uv.x * 7.3 + timer * 0.45)) / 3.0;
    uv.x += 0.12 * (sin(uv.y * 4.0 + timer * 0.5) + sin(uv.y * 6.8 + timer * 0.75) + sin(uv.y * 11.3 + timer * 0.2)) / 3.0;
    uv.y += 0.12 * (sin(uv.x * 4.2 + timer * 0.64) + sin(uv.x * 6.3 + timer * 1.65) + sin(uv.x * 8.2 + timer * 0.45)) / 3.0;

    vec4 tex1 = texture2D(tex, uv * 1.0);
    vec4 tex2 = texture2D(tex, uv * 1.0 + vec2(0.2));

    vec3 blue = vec3(0, 0.5, 1.0);
	vec4 celColor;
	vec3 normal = normalize(norm);	
	vec3 viewDir = -normalize(fragPos);			
	Light l = light;
	l.position = vec3(camView * vec4(light.position, 1.0));
	celColor = vec4(CalcLight(l, normal, fragPos).rgb, fragColor.a);	
	if(!noTex) {		
		color = celColor * vec4(blue + vec3(tex1.a * 0.5 - tex2.a * 0.02), fragColor.a);	
	}
	else {
		color = celColor * vec4(blue, 1.0f);
	}
		
}
