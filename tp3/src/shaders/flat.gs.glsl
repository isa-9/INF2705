#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in ATTRIB_OUT
{
    vec3 position;
    vec2 texCoords;
} attribIn[3];

out ATTRIB_VS_OUT
{
    vec2 texCoords;    
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} attribOut;

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

float calculerSpot( in vec3 D, in vec3 L, in vec3 N )
{
    float spotFacteur = 0.0;
    if ( dot( D, N ) >= 0 )
    {
        float spotDot = dot( L, D );
        float cosDelta = cos(radians(lightingBlock.spotOpeningAngle));
        if ( spotDot > cosDelta ) {
            spotFacteur = lightingBlock.useDirect3D ? 
                smoothstep( pow( cosDelta, 1.01 + lightingBlock.spotExponent / 2), cosDelta, spotDot )
            : pow( spotDot, lightingBlock.spotExponent );
        }
    }
    return( spotFacteur );
}

void main()
{
	// TODO
    attribOut.texCoords = attribIn[0].texCoords;

    vec3 side1 = ( attribIn[1].position - attribIn[0].position );
    vec3 side2 = ( attribIn[2].position - attribIn[0].position );
    vec3 normal = normalMatrix * cross ( side1 , side2 );

   	vec3 faceCentroid = (attribIn[0].position + attribIn[1].position + attribIn[2].position) / 3.0;

    vec3 pos = vec3(modelView * vec4(faceCentroid, 1));

    vec3 lightDir[3];

    lightDir[0] = (view * vec4(lightingBlock.lights[0].position, 1.0f)).xyz - pos;
    lightDir[1] = (view * vec4(lightingBlock.lights[1].position, 1.0f)).xyz - pos;
    lightDir[2] = (view * vec4(lightingBlock.lights[2].position, 1.0f)).xyz - pos;

    vec3 obs = -pos;

    vec3 faceColor = lightingBlock.mat.emission;

    vec3 O = normalize(-pos);

    vec3 L[3];
    L[0] = normalize(lightDir[0]);
    L[1] = normalize(lightDir[1]);
    L[2] = normalize(lightDir[2]);

    vec3 D[3];
    D[0] = mat3(view) * -lightingBlock.lights[0].spotDirection;
    D[1] = mat3(view) * -lightingBlock.lights[1].spotDirection;
    D[2] = mat3(view) * -lightingBlock.lights[2].spotDirection;

    vec3 N = normalize(normal);

	vec3 ambientNoMat = lightingBlock.lightModelAmbient;
    ambientNoMat += lightingBlock.lights[0].ambient;
    ambientNoMat += lightingBlock.lights[1].ambient;
    ambientNoMat += lightingBlock.lights[2].ambient;

    attribOut.ambient = lightingBlock.mat.ambient * ambientNoMat;

    vec3 diffuse = lightingBlock.lights[0].diffuse * max(dot(L[0],N), 0) * (lightingBlock.useSpotlight ? calculerSpot( D[0], L[0], N ) : 1 );
    diffuse += lightingBlock.lights[1].diffuse * max(dot(L[1],N), 0) * (lightingBlock.useSpotlight ? calculerSpot( D[1], L[1], N ) : 1 );
    diffuse += lightingBlock.lights[2].diffuse * max(dot(L[2],N), 0) * (lightingBlock.useSpotlight ? calculerSpot( D[2], L[2], N ) : 1 );
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

    vec3 spec = (spectIntensity[0] > 0 && dot(L[0],N) > 0 ? lightingBlock.lights[0].specular * shine[0] : vec3(0.0f))
                     * (lightingBlock.useSpotlight ? calculerSpot( D[0], L[0], N ) : 1 );

    spec += (spectIntensity[1] > 0 && dot(L[1],N) > 0 ? lightingBlock.lights[1].specular * shine[1] : vec3(0.0f))
                     * (lightingBlock.useSpotlight ? calculerSpot( D[1], L[1], N ) : 1 );

    spec += (spectIntensity[2] > 0 && dot(L[2],N) > 0 ? lightingBlock.lights[2].specular * shine[2] : vec3(0.0f))
                     * (lightingBlock.useSpotlight ? calculerSpot( D[2], L[2], N ) : 1 );

    spec *= lightingBlock.mat.specular;
   
    attribOut.specular = spec;
    attribOut.emission = lightingBlock.mat.emission;
    
    for (int i = 0 ; i < gl_in.length() ; i++) {
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}
