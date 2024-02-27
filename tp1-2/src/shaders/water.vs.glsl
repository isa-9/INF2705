#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

uniform mat4 mvp;

out vec2 fragTexCoords;

void main() {
	vec4 transformed_position = mvp * vec4(position, 1.0);
	gl_Position = transformed_position;

    fragTexCoords = texCoords;
}