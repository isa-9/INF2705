#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;


in ATTRIB_TES_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
} attribIn[];

out ATTRIB_GS_OUT
{
    float height;
    vec2 texCoords;
    vec4 patchDistance;
    vec3 barycentricCoords;
} attribOut;

void main()
{
    // TODO
    vec3 barycentricCoords[] = vec3[](vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1));

    for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		attribOut.texCoords = attribIn[i].texCoords;
		attribOut.patchDistance = attribIn[i].patchDistance;
		attribOut.height = attribIn[i].height;
        attribOut.barycentricCoords = barycentricCoords[i];
		EmitVertex();
	}
}
