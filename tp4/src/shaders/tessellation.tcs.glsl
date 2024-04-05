#version 400 core

layout(vertices = 4) out;

uniform mat4 modelView;


const float MIN_TESS = 4;
const float MAX_TESS = 64;

const float MIN_DIST = 30.0f;
const float MAX_DIST = 100.0f;

void main()
{
	// TODO
    gl_in[0].gl_Position; // (0,0)
    gl_in[1].gl_Position; // (1,0)
    gl_in[2].gl_Position; // (1,1)
    gl_in[3].gl_Position; // (0,1)

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;


    vec4 centre[4];
    centre[0] = (gl_in[0].gl_Position + gl_in[3].gl_Position) * 0.5;
    centre[1] = (gl_in[0].gl_Position + gl_in[1].gl_Position) * 0.5;
    centre[2] = (gl_in[1].gl_Position + gl_in[2].gl_Position) * 0.5;
    centre[3] = (gl_in[2].gl_Position + gl_in[3].gl_Position) * 0.5;

    float distance[4];
    distance[0] = length(modelView * centre[0]);
    distance[1] = length(modelView * centre[1]);
    distance[2] = length(modelView * centre[2]);
    distance[3] = length(modelView * centre[3]);

    float facteurMix[4];
    facteurMix[0] = smoothstep(MIN_DIST, MAX_DIST, clamp(distance[0], MIN_DIST, MAX_DIST));
    facteurMix[1] = smoothstep(MIN_DIST, MAX_DIST, clamp(distance[1], MIN_DIST, MAX_DIST));
    facteurMix[2] = smoothstep(MIN_DIST, MAX_DIST, clamp(distance[2], MIN_DIST, MAX_DIST));
    facteurMix[3] = smoothstep(MIN_DIST, MAX_DIST, clamp(distance[3], MIN_DIST, MAX_DIST));

    gl_TessLevelOuter[0] = mix(MIN_TESS, MAX_TESS, 1 - facteurMix[0]);
    gl_TessLevelOuter[1] = mix(MIN_TESS, MAX_TESS, 1 - facteurMix[1]);
    gl_TessLevelOuter[2] = mix(MIN_TESS, MAX_TESS, 1 - facteurMix[2]);
    gl_TessLevelOuter[3] = mix(MIN_TESS, MAX_TESS, 1 - facteurMix[3]);

    float facteurInt = max(max(max(gl_TessLevelOuter[0], gl_TessLevelOuter[1]), gl_TessLevelOuter[2]), gl_TessLevelOuter[3]);

    gl_TessLevelInner[0] = facteurInt;
    gl_TessLevelInner[1] = facteurInt;
    // gl_TessLevelInner[2] = facteurInt;
    // gl_TessLevelInner[3] = facteurInt;
}
