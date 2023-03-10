#version 400
subroutine vec4 RenderPassType();
subroutine uniform RenderPassType RenderPass;

in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec2 fragTex;
in vec3 viewPos;
in mat4 camView;

uniform sampler2D tex;
uniform sampler2D renderTex;
uniform vec4 inCol;

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
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {	
	vec3 lightDir = normalize(-light.direction.xyz);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0));
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));			
	float attenunation = 1.0f;	
	return (light.ambient + light.diffuse * diff + light.specular * spec);
}
vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 positionToLightSource = light.position - fragPos;
	vec3 lightDir = normalize(positionToLightSource);
	float diff = CalcDiffuse(max(dot(normal, lightDir), 0.0f));
	vec3 reflectDir = reflect(-lightDir, normal);	
	float spec = CalcSpec(pow(max(dot(viewDir, reflectDir), 0.0), 0.0f));		
	float dist = length(light.position - fragPos);
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
	return (light.ambient + light.diffuse * diff + light.specular * spec) * attenunation;
}

float luma(vec3 color) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}
subroutine(RenderPassType) 
vec4 DrawModel() {
	vec4 celColor;
	if(inCol.a == 0.0) {
		vec3 normal = normalize(norm);	
		vec3 viewDir = normalize(viewPos - fragPos);		
		if(fragColor.a < 0) {
			celColor = fragColor;
		}
		else {
			for(int i = 0; i < numLights; i++) {		
				Light light = lights[i];	
				light.position = vec3(camView * vec4(light.position, 1.0));		
				if(light.direction.w == 0) {
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
		
		return celColor * texture(tex, vec2(fragTex.x, -fragTex.y)) * fragColor;	
	}
	else {
		return inCol;
	}
}

subroutine( RenderPassType )
vec4 DrawOutline() {
	float dx = 1.0 / float(width);
	float dy = 1.0 / float(height);
	float s00 = luma(texture(renderTex,
					 fragTex + vec2(-dx,dy) ).rgb);
	float s10 = luma(texture(renderTex,
					 fragTex + vec2(-dx,0.0) ).rgb);
	float s20 = luma(texture(renderTex,
					 fragTex + vec2(-dx,-dy) ).rgb);
	float s01 = luma(texture(renderTex,
					 fragTex + vec2(0.0,dy) ).rgb);
	float s21 = luma(texture(renderTex,
					 fragTex + vec2(0.0,-dy) ).rgb);
	float s02 = luma(texture(renderTex,	
					 fragTex + vec2(dx, dy) ).rgb);
	float s12 = luma(texture(renderTex,
					 fragTex + vec2(dx, 0.0) ).rgb);
	float s22 = luma(texture(renderTex,
					 fragTex + vec2(dx, -dy) ).rgb);
	float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
	float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
	float dist = sx * sx + sy * sy;
	if(dist> 0.0001f)
		return vec4(0.0);
	else
		return vec4(0.0, 0.0, 0.0, 1.0);
}
void main() {	
	color = RenderPass();	
}
