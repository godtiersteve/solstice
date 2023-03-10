#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 tex_mat;

out vec2 frag_tex;

void main()
{
    gl_Position = projection * view * model * vec4(position, 0.0f, 1);
	vec3 tex = vec3(texcoord.xy, 1) * tex_mat;
	frag_tex = tex.xy;
} 