#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 mvp;

void main()
{
    vec3 posWithNormal = position + 0.1 * normal;
    gl_Position = mvp * vec4(posWithNormal, 1.0f);
}
