#version 330 core

layout(location = 1) in vec3 color;
out vec3 fragColor;

void main() {
	fragColor = color;
}