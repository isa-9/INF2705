#version 330 core

in vec2 fragTexCoords;

uniform sampler2D tex0;
uniform float time;

out vec4 outColor;

void main() {
	// Calcul des nouvelles coordonnées de texture
	float offsetX = cos(time * 1.0 + (fragTexCoords.x + fragTexCoords.y) * 2.0) * 0.025f + time / 10;
    float offsetY = sin(time * 2.0 + (fragTexCoords.x + fragTexCoords.y) * 2.0) * 0.01f;

	outColor = texture(tex0, fragTexCoords + vec2(offsetX, offsetY));

	if (outColor.a < 0.3)
		discard;
}