#version 330
in vec4 fragColor;
in vec3 Normal;
in vec3 fragPos;
in vec3 lightPos;
in vec2 fragTex;

uniform sampler2D tex;
uniform bool hasTexture;

out vec4 color;
void main() {
	vec3 lightColor = vec3(1.0f);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	
	vec3 viewPos = lightPos;
	float specularStrength = 0.125f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;	
	
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;		
	
	float ambientStrength = 0.4f;
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = (diffuse + ambient) * fragColor.rgb;
	
	if(hasTexture) {
		color = vec4(result, fragColor.a) *  texture(tex, vec2(fragTex.x, -fragTex.y));
	}
	else {
		color = vec4(result, fragColor.a);
	}
	//color = vec4(fragColor.rgb, 0.5f);
}