#version 330 core

in vec3 texCoords;

uniform samplerCube tex;

out vec4 outColor;

void main() {
	outColor = texture(tex, vec2(texCoords.x, texCoords.y));

	if (outColor.a < 0.3)
		discard;
}