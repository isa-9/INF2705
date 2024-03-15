#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

    vec3 N = normalize(normalMatrix * normal);

    vec3 pos = vec3(modelView * vec4(position, 1.0f));

    vec3 lightDir[3];

    lightDir[0] = (view * vec4(lightingBlock.lights[0].position, 1.0f)).xyz - pos;
    lightDir[1] = (view * vec4(lightingBlock.lights[1].position, 1.0f)).xyz - pos;
    lightDir[2] = (view * vec4(lightingBlock.lights[2].position, 1.0f)).xyz - pos;

    vec3 O = normalize(-pos);

    vec3 L[3];
    L[0] = normalize(lightDir[0]);
    L[1] = normalize(lightDir[1]);
    L[2] = normalize(lightDir[2]);

    vec3 ambientNoMat = lightingBlock.lightModelAmbient;
    ambientNoMat += lightingBlock.lights[0].ambient;
    ambientNoMat += lightingBlock.lights[1].ambient;
    ambientNoMat += lightingBlock.lights[2].ambient;

    attribOut.ambient = lightingBlock.mat.ambient * ambientNoMat;
     
    vec3 diffuse = lightingBlock.lights[0].diffuse * dot(L[0],N);
    diffuse+= lightingBlock.lights[1].diffuse * dot(L[1],N);
    diffuse+= lightingBlock.lights[2].diffuse * dot(L[2], N);
    diffuse *= lightingBlock.mat.diffuse;

    attribOut.diffuse = diffuse;


    float spectIntensity[3];
    spectIntensity[0] = lightingBlock.useBlinn ? dot(normalize(L[0] + O), N) : dot(reflect(-L[0], N), O);
    spectIntensity[1] = lightingBlock.useBlinn ? dot(normalize(L[1] + O), N) : dot(reflect(-L[1], N), O);
    spectIntensity[2] = lightingBlock.useBlinn ? dot(normalize(L[2] + O), N) : dot(reflect(-L[2], N), O);

    float shine[3];
    shine[0] = pow(spectIntensity[0], lightingBlock.mat.shininess);
    shine[1] = pow(spectIntensity[1], lightingBlock.mat.shininess);
    shine[2] = pow(spectIntensity[2], lightingBlock.mat.shininess);


    vec3 spec = spectIntensity[0] > 0 ? lightingBlock.lights[0].specular * shine[0] : vec3(0.0f);
    spec += spectIntensity[1] > 0 ? lightingBlock.lights[1].specular * shine[1] : vec3(0.0f);
    spec += spectIntensity[2] > 0 ? lightingBlock.lights[2].specular * shine[2] : vec3(0.0f);
    spec *= lightingBlock.mat.specular;

    attribOut.specular = spec;
    attribOut.emission = lightingBlock.mat.emission;
}
