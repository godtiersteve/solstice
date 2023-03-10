#version 420

in vec4 fragColor;
in vec2 texCoords;
in vec3 fragPos;
out vec4 color;

//uniform sampler2D tex;

void main()
{			
	color = texture(tex, vec2(texCoords.x, -texCoords.y)) * fragColor;
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}