#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec2 fragTex;
in vec3 viewPos;
in mat4 camView;

uniform sampler2D textures[20];
uniform sampler2D texture;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec4 direction;
};
uniform Light lights[20];
uniform int numLights;

out vec4 color;
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
float fmod(float a, float b) {
	return a - b * trunc(a / b);
}
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction.xyz);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0));
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));		
	float distance = length(light.position - fragPos);
	float attenunation = 1.0f / distance;
	if(light.direction.w <= 90.0) {
		float clampedCosine = max(0.0, dot(-lightDir, light.direction.xyz));
		if(clampedCosine < light.direction.w) {
			attenunation = 0.0f;
		}
		else {
			attenunation = attenunation * pow(clampedCosine, 2.0f);
		}
	}
	return (light.ambient + light.diffuse * diff + light.specular * spec);
}
vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0f));
	vec3 reflectDir = reflect(-lightDir, normal);	
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));		
	float dist = length(light.position - fragPos);
	//float attenunation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);
	float attenunation = 1.0 / (light.direction.x + light.direction.y * dist + light.direction.z * dist * dist);	
	return (light.ambient + light.diffuse * diff + light.specular * spec) * attenunation;
}
void main() {	
	vec4 celColor;
	vec3 normal = normalize(norm);	
	vec3 viewDir = normalize(viewPos - fragPos);		
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
				celColor+= vec4(CalcPointLight(light, normal, viewDir), 0.0f);	
			}
		}
		celColor.a = fragColor.a;
	}
	
	if(fragTex.x < -1000) {
		color = celColor * fragColor;		
	}
	else {	
		float xTex = mod(fragTex.x, 10000);
		int offset = int((fragTex.x - xTex) / 10000) - 1;	
		if(offset < 0) {
			offset = 0;
		}
		vec2 tex = vec2(xTex, -fragTex.y);
		color = celColor * texture(textures[offset], tex) * fragColor;
	}		
}
