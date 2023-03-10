#version 330
in vec4 fragColor;
in vec3 norm;
in vec3 fragPos;
in vec3 lightPos;
in vec2 fragTex;

uniform sampler2D tex;
uniform bool hasTexture;

out vec4 color;
const vec3 ambientColor = vec3(0.2, 0.2, 0.2);
const vec3 diffuseColor = vec3(0.25, 0.25, 0.25);
const vec3 specColor = vec3(0.7, 0.7, 0.7);

void main() {
    vec3 normal = normalize(norm);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-fragPos);

    float lambertian = max(dot(lightDir, normal), 0.0);	
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(reflectDir, viewDir), 0.0);
       specular = pow(specAngle, 4.0);
    }
    vec3 result = vec3(ambientColor +
                      lambertian * diffuseColor +
                      specular*specColor);
	float intensity = dot(lightDir, normal);
	vec4 celColor;
	float intense;
	if(intensity < 0) {
		intensity = 0;
	}
	if(intensity > 0.95f) {
		intense = 1.0f;
	}
	else if(intensity > 0.85f) {
		intense = 0.65f;
	}
	else if(intensity > 0.5f) {
		intense = 0.30f;
	}
	else {
		intense = 0.5f;
	}
	celColor = vec4(intense * vec3(result), fragColor.a);
	//celColor = vec4(result, fragColor.a);
	if(hasTexture) {
		color = celColor *  texture(tex, vec2(fragTex.x, -fragTex.y));
	}
	else {
		color = celColor;
	}	
}
