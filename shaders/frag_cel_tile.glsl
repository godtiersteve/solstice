#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec2 fragTex;
in vec3 viewPos;
in mat4 camView;

uniform sampler2D tex;
uniform vec4 inCol;
uniform vec2 textureScale;
uniform vec2 texOffset;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec4 direction;	
	float specExponent;
	float spotCutoff, linear, constant, quadratic;
};
uniform Light lights[20];
uniform int numLights;
const float PI_OVER_2 = 1.57079632679;
out vec4 color;
//out vec4 fragNorm;
//out vec4 renderPos;

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
	else if(light.direction.w != 0.0 && light.spotCutoff > PI_OVER_2) {
		vec3 positionToLightSource = light.position - fragPos;
		float distance = length(positionToLightSource);
		lightDirection = normalize(positionToLightSource);
		attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	}
	else if(light.direction.w != 0.0 && light.spotCutoff <= PI_OVER_2) {
		vec3 positionToLightSource = light.position - fragPos;
		float distance = length(positionToLightSource);		
		lightDirection = normalize(positionToLightSource);
		float theta = dot(lightDirection, normalize(-light.direction.xyz));
		if(theta > light.spotCutoff) {
			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
		}
		else {
			attenuation = 0.0f;
		}
	}
	float diff = CalcDiffuse(max(dot(normal, lightDirection), 0.0f));
	vec3 reflectDir = reflect(-lightDirection, normal);	
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = CalcSpec(pow(max(dot(normal, halfwayDir), 0.0), light.specExponent));	
	return light.ambient + (light.diffuse * diff + light.specular * spec) * attenuation;
}
void main() {	
	vec4 celColor;
	
	if(inCol.a == 0.0 && fragColor.a > 0.0) {
		vec3 normal = normalize(norm);	
		vec3 viewDir = normalize(viewPos - fragPos);		
		if(fragColor.a < 0) {
			celColor = fragColor;
		}
		else {
			for(int i = 0; i < numLights; i++) {					
				Light light = lights[i];	
				if(light.ambient.r >= 0.0) {
					if(light.direction.w != 0.0) {
						light.position = vec3(camView * vec4(light.position, 1.0));	
					}
					if(light.direction.w == 0.0 || light.spotCutoff <= 90) {
						light.direction.xyz = vec3(transpose(inverse(camView)) * vec4(light.direction.xyz, 1.0f));
					}
					celColor+= vec4(CalcLight(light, normal, viewDir), 0.0f);	
				}
			}
			celColor.a = fragColor.a;
		}
		vec2 texCoord = fragTex;
		//texCoord.x = fract(fragTex.x) * texOffset.x;
		//texCoord.x = mod(fragTex.x * textureScale.x , texOffset.x) * fragTex.x + 0.25;
		//texCoord.x = mod(fragTex.x * textureScale.x , texOffset.x) * fragTex.x + 0.25;
		//texCoord.y = mod(fragTex.y, 1.0) * fragTex.y;
		texCoord.y = -texCoord.y;
		color = celColor * texture(tex, texCoord) * fragColor;	
		//color = celColor * texture(tex, vec2(fragTex.x  * textureScale.x, -fragTex.y  * textureScale.y)) * fragColor;	
	}
	else {
		if(fragColor.a < 0.0) {
			color = texture(tex, vec2(fragTex.x * textureScale.x, -fragTex.y * textureScale.x)) * fragColor;
			color.a = -color.a;
		}
		else {
			color = inCol;		
		}
	}
	
	//color = texture(tex, vec2(fragTex.x, -fragTex.y)) * fragColor;
	//color = fragColor;
	//fragNorm = vec4(norm, 1.0f);
	//renderPos = vec4(fragPos, 1.0f);
}
