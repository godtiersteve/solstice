#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
//in vec3 lightPos;
in vec2 fragTex;

uniform sampler2D tex;
uniform bool hasTexture;
uniform bool noLight;
uniform sampler2D textures[20];

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	vec3 direction;
};
uniform Light light;

out vec4 color;
float stepmix(float edge0, float edge1, float E, float x)  {
	float t = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
	return mix(edge0, edge1, t);
}
void main() {	
	vec4 celColor;
	if(!noLight) {
		float dist = distance(light.position, fragPos);
		float attenunation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);
		vec3 normal = normalize(norm);
		vec3 lightDir = normalize(light.position - fragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-fragPos);
		
		float df = max(dot(lightDir, normal), 0.0);		
		float spec = max(dot(reflectDir, viewDir), 0.0);	

		const float A = 0.1f;
		const float B = 0.3f;
		const float C = 0.6f;
		const float D = 1.0f;
		
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
		
		E = fwidth(spec);
		if(spec > 0.5 - E && spec < 0.5 + E) {
			spec = clamp(0.5 * (spec - 0.5 + E) / E, 0.0, 1.0);
		}
		else {
			spec = step(0.5f, spec);
		}
		vec3 result = vec3((light.ambient +
						   df * light.diffuse +
						   spec * light.specular) * attenunation);
		celColor = vec4(result, fragColor.a);	
	}
	else {
		celColor = fragColor;	
	}
	if(hasTexture) {
		if(fragTex.x < 0) {
			color = celColor *  texture(tex, vec2(fragTex.x, -fragTex.y));		
		}
		else {	
			float xTex = mod(fragTex.x, 1000);
			int offset = int((fragTex.x - xTex) / 1000) - 1;	
			vec2 tex = vec2(xTex, -mod(fragTex.y, 1000));
			color = celColor * texture(textures[offset], tex) * fragColor;
		}		
	}
	else {
		color = celColor * fragColor;
	}	
}
