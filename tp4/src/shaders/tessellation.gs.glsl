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
    vec4 barycentre = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;

    for (int i = 0; i < gl_in.length(); i++) {
		gl_Position = gl_in[i].gl_Position;
		attribOut.texCoords = attribIn[i].texCoords;
		attribOut.patchDistance = attribIn[i].patchDistance;
		attribOut.height = attribIn[i].height;
        attribOut.barycentricCoords = vec3(barycentre);
		EmitVertex();
	}
}
