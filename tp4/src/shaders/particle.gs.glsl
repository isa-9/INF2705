#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in ATTRIB_VS_OUT
{
    vec4 color;
    vec2 size;
} attribIn[];

out ATTRIB_GS_OUT
{
    vec4 color;
    vec2 texCoords;
} attribOut;

uniform mat4 projection;

void main()
{
    // TODO
	vec4 center = gl_in[0].gl_Position;
	vec2 apothem = attribIn[0].size / 2;
	vec4 addedPosition = vec4(-apothem, 0.0f, 0.0f);

	gl_Position = projection * (center + addedPosition);
	attribOut.color = attribIn[0].color;
	attribOut.texCoords = vec2(0,1);//normalize(center + addedPosition).xy;
	EmitVertex();

	addedPosition = vec4(apothem.x, -apothem.y, 0.0f, 0.0f);
	gl_Position = projection * (center + addedPosition);
	attribOut.color = attribIn[0].color;
	attribOut.texCoords = vec2(1,1);//normalize(center + addedPosition).xy;
	EmitVertex();

	addedPosition = vec4(-apothem.x, apothem.y, 0.0f, 0.0f);
	gl_Position = projection * (center + addedPosition);
	attribOut.color = attribIn[0].color;
	attribOut.texCoords = vec2(0,0);//normalize(center + addedPosition).xy;
	EmitVertex();

	addedPosition = vec4(apothem, 0.0f, 0.0f);
	gl_Position = projection * (center + addedPosition);
	attribOut.color = attribIn[0].color;
	attribOut.texCoords = vec2(1,0);// normalize(center + addedPosition).xy;
	EmitVertex();

	EndPrimitive();
}
