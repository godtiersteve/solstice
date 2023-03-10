in vec2 uv;
out vec4 color;
uniform sampler2D renderedTexture;
void main() {
	color = texture(renderedTexture, vec2(uv.x., -uv.y));
}
