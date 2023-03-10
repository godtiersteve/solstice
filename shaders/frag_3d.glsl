#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec3 lightPos;
in vec2 fragTex;

uniform sampler2D tex;
uniform bool hasTexture;

out vec4 color;
float stepmix(float edge0, float edge1, float E, float x)  {
	float t = clamp(0.5 * (x - edge0 + E) / E, 0.0, 1.0);
	return mix(edge0, edge1, t);
}
void main() {	
	struct Light {
		vec3 direction;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	} light;
	light.direction = vec3(0.0f, 2.0f, -1.0f);
	light.ambient = vec3(0.25f);
	light.diffuse = vec3(0.50f);
	light.specular = vec3(0.25f);

    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-fragPos);
	
    float df = max(dot(lightDir, normal), 0.0);		
    float spec = max(dot(reflectDir, viewDir), 0.0);
	spec = pow(spec, 4.0f);

    vec3 result = vec3(light.ambient +
                      df * light.diffuse +
                      spec * light.specular);
	
	vec4 celColor = vec4(result, fragColor.a);	
	if(hasTexture) {
		color = celColor *  texture(tex, vec2(fragTex.x, -fragTex.y));
	}
	else {
		color = celColor;
	}	
}
