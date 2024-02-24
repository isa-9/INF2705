#version 330 core

in vec2 fragTexCoords;

uniform sampler2D tex0;

out vec4 outColor;

void main() {
	outColor = texture(tex0, fragTexCoords);

	if (outColor.a < 0.3)
		discard;
}