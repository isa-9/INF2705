#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribOut;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 modelView;
uniform mat3 normalMatrix;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
} lightingBlock;

void main()
{
    // TODO
    attribOut.texCoords = texCoords;
    vec4 normPosition = mvp * vec4(position, 1.0f);
    gl_Position = normPosition;

    attribOut.normal = normalMatrix * normal;

    vec3 pos = vec3(modelView * vec4(position, 1.0f));

    attribOut.lightDir[0] = (view * vec4(lightingBlock.lights[0].position, 1.0f)).xyz - pos;
    attribOut.lightDir[1] = (view * vec4(lightingBlock.lights[1].position, 1.0f)).xyz - pos;
    attribOut.lightDir[2] = (view * vec4(lightingBlock.lights[2].position, 1.0f)).xyz - pos;

    attribOut.obsPos = -pos;

    attribOut.spotDir[0] = mat3(view) * -lightingBlock.lights[0].spotDirection;
    attribOut.spotDir[1] = mat3(view) * -lightingBlock.lights[1].spotDirection;
    attribOut.spotDir[2] = mat3(view) * -lightingBlock.lights[2].spotDirection;
}
