#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

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

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

vec4 calculerReflexion( in vec3 L, in vec3 N, in vec3 O, in int i )
{
    vec3 coul = vec3(0);
    coul += lightingBlock.mat.ambient * lightingBlock.lights[i].ambient;

    // calculer l'éclairage seulement si le produit scalaire est positif
    float NdotL = max( 0.0, dot( N, L ) );
    if ( NdotL > 0.0 )
    {
        // calculer la composante diffuse
        coul += lightingBlock.mat.diffuse * lightingBlock.lights[i].diffuse * NdotL;

        // calculer la composante spéculaire (Blinn ou Phong : spec = BdotN ou RdotO )
        float spec = ( lightingBlock.useBlinn ?
                       dot( normalize( L + O ), N ) : // dot( B, N )
                       dot( reflect( -L, N ), O ) ); // dot( R, O )
        if ( spec > 0 ) coul +=  lightingBlock.mat.specular * lightingBlock.lights[i].specular * pow( spec, lightingBlock.mat.shininess );
    }

    return( vec4(coul, 0.0f) );
}



void main()
{
    // TODO
    FragColor = vec4(lightingBlock.mat.emission, 1.0f);

    vec3 L = normalize(attribIn.lightDir[0]);

    vec3 N = normalize( gl_FrontFacing ? attribIn.normal : -attribIn.normal );

    vec3 O = normalize(attribIn.obsPos);

    vec4 col = vec4(lightingBlock.mat.emission + lightingBlock.mat.ambient * lightingBlock.lightModelAmbient, 1.0f);

    col += calculerReflexion( L, N, O, 0 );
    
    L = normalize(attribIn.lightDir[1]);
    col += calculerReflexion( L, N, O, 1 );
    
    L = normalize(attribIn.lightDir[2]);
    col += calculerReflexion( L, N, O, 2 );

    FragColor = clamp( col, 0.0, 1.0 );
}
