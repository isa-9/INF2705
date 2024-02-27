#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 mvp;

out vec3 texCoords;

void main() {
	vec4 transformed_position = mvp * vec4(position.x, position.y, position.z, 1.0);
	gl_Position = transformed_position.xyww;
	texCoords = position;
}