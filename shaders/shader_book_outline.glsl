	float dx = 1.0f / WIDTH;
	float dy = 1.0f / HEIGHT;
	float s00 = luma(texture(tex, fragTex + vec2(-dx, dy)).rgb);
	float s10 = luma(texture(tex, fragTex + vec2(-dx, 0.0)).rgb);
	float s20 = luma(texture(tex, fragTex + vec2(-dx, -dy)).rgb);
	float s01 = luma(texture(tex, fragTex + vec2(0.0, dy)).rgb);
	float s21 = luma(texture(tex, fragTex + vec2(0.0, -dy)).rgb);
	float s02 = luma(texture(tex, fragTex + vec2(dx, dy)).rgb);
	float s12 = luma(texture(tex, fragTex + vec2(dx, 0.0)).rgb);
	float s22 = luma(texture(tex, fragTex + vec2(dx, -dy)).rgb);	
	float sx = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
	float sy = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
	float dist = sx * sx + sy * sy;
	
	if(dist > edgeThreshold) {
		return celColor * vec4(vec3(0.0), 1.0) * texture(tex, fragTex);
	}
	else {
		return celColor * texture(tex, fragTex);
	}	