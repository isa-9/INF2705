#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 mvp;
uniform float time;

out vec2 fragTexCoords;

void main() {
	vec4 transformed_position = mvp * vec4(position, 1.0);
	gl_Position = transformed_position;

	//fragTexCoords.x = 0.025f * cos(time / 10 * 1 + (texCoords.x + texCoords.y) * 2);
	//fragTexCoords.y = 0.01f * sin(time / 10 * 3 + (texCoords.x + texCoords.y) * 2);

	float offsetX = cos(time * 1.0 + (texCoords.x + texCoords.y) * 2.0) * 0.025f;
    float offsetY = sin(time * 2.0 + (texCoords.x + texCoords.y) * 2.0) * 0.01f;

    // Calcul des nouvelles coordonnées de texture
    fragTexCoords = texCoords + vec2(offsetX, offsetY);
}